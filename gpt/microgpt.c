#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

/* --- Configuration --- */
#define N_EMBD 16
#define N_HEAD 4
#define N_LAYER 1
#define BLOCK_SIZE 16
#define HEAD_DIM (N_EMBD / N_HEAD)
#define MAX_DOCS 1000
#define MAX_DOC_LEN 100
#define MAX_NODES 1000000  // Size of the computation graph tape
#define MAX_PARAMS 10000   // Max number of model parameters

/* --- Autograd Engine --- */

typedef enum { OP_CONST, OP_ADD, OP_MUL, OP_POW, OP_LOG, OP_EXP, OP_RELU } OpType;

typedef struct Value {
    float data;
    float grad;
    struct Value* prev[2]; // Children in the graph
    OpType op;
    float aux; // Auxiliary data (e.g., exponent for POW)
    bool visited; // For topological sort
} Value;

// The "Tape" - A static memory arena for the computation graph
Value node_pool[MAX_NODES];
int node_idx = 0;

// Parameter storage (these persist across steps, unlike the graph nodes)
Value* params[MAX_PARAMS];
int param_count = 0;
// Adam Optimizer buffers
float adam_m[MAX_PARAMS];
float adam_v[MAX_PARAMS];

// Reset the graph memory for a new step
void zero_tape() {
    node_idx = 0;
}

// Allocate a new node on the tape
Value* new_value(float data) {
    if (node_idx >= MAX_NODES) {
        printf("Error: Computation graph memory exhausted.\n");
        exit(1);
    }
    Value* v = &node_pool[node_idx++];
    v->data = data;
    v->grad = 0.0f;
    v->prev[0] = NULL;
    v->prev[1] = NULL;
    v->op = OP_CONST;
    v->visited = false;
    return v;
}

// Create a persistent parameter (weight)
Value* new_param(float data) {
    Value* v = malloc(sizeof(Value));
    v->data = data;
    v->grad = 0.0f;
    v->prev[0] = NULL;
    v->prev[1] = NULL;
    v->op = OP_CONST;
    
    if (param_count < MAX_PARAMS) {
        params[param_count] = v;
        adam_m[param_count] = 0.0f;
        adam_v[param_count] = 0.0f;
        param_count++;
    }
    return v;
}

/* --- Operations --- */

Value* add(Value* a, Value* b) {
    Value* v = new_value(a->data + b->data);
    v->prev[0] = a; v->prev[1] = b; v->op = OP_ADD;
    return v;
}

Value* sub(Value* a, Value* b) {
    Value* neg_b = new_value(-b->data); // Creating intermediate node
    neg_b->prev[0] = b; neg_b->prev[1] = NULL; neg_b->op = OP_MUL; neg_b->aux = -1.0f; // Hack: mul by constant -1
    return add(a, neg_b);
}

Value* mul(Value* a, Value* b) {
    Value* v = new_value(a->data * b->data);
    v->prev[0] = a; v->prev[1] = b; v->op = OP_MUL;
    return v;
}

Value* power(Value* a, float exponent) {
    Value* v = new_value(powf(a->data, exponent));
    v->prev[0] = a; v->op = OP_POW; v->aux = exponent;
    return v;
}

Value* vlog(Value* a) {
    Value* v = new_value(logf(a->data));
    v->prev[0] = a; v->op = OP_LOG;
    return v;
}

Value* vexp(Value* a) {
    Value* v = new_value(expf(a->data));
    v->prev[0] = a; v->op = OP_EXP;
    return v;
}

Value* relu(Value* a) {
    Value* v = new_value(a->data > 0 ? a->data : 0);
    v->prev[0] = a; v->op = OP_RELU;
    return v;
}

// Special case for mul by constant to save graph nodes
Value* mul_const(Value* a, float c) {
    Value* v = new_value(a->data * c);
    v->prev[0] = a; v->op = OP_MUL; // Handled as binary op where b is implicit or handled in backprop logic specially
    // For simplicity in this C port, we won't optimize const mul backprop deeply,
    // we will just create a const node.
    Value* c_val = new_value(c);
    v->prev[1] = c_val;
    return v;
}

/* --- Backward Pass --- */

Value* topo[MAX_NODES];
int topo_idx = 0;

void build_topo(Value* v) {
    if (v->visited) return;
    v->visited = true;
    if (v->prev[0]) build_topo(v->prev[0]);
    if (v->prev[1]) build_topo(v->prev[1]);
    topo[topo_idx++] = v;
}

void backward(Value* root) {
    // 1. Build topological order
    topo_idx = 0;
    // Reset visited flags in the pool is costly, but necessary if we reuse nodes (we don't here per step)
    // Since we reset node_idx every step, visited flags are implicitly garbage. 
    // We must ensure new_value inits visited to false.
    
    // We also need to traverse params.
    for(int i=0; i<param_count; i++) params[i]->visited = false;
    
    build_topo(root);
    
    // 2. Set root gradient
    root->grad = 1.0f;
    
    // 3. Process in reverse topological order
    for (int i = topo_idx - 1; i >= 0; i--) {
        Value* v = topo[i];
        if (v->op == OP_CONST) continue;
        
        Value* child1 = v->prev[0];
        Value* child2 = v->prev[1];
        
        switch (v->op) {
            case OP_ADD:
                child1->grad += v->grad;
                child2->grad += v->grad;
                break;
            case OP_MUL:
                child1->grad += child2->data * v->grad;
                child2->grad += child1->data * v->grad;
                break;
            case OP_POW:
                child1->grad += (v->aux * powf(child1->data, v->aux - 1)) * v->grad;
                break;
            case OP_LOG:
                child1->grad += (1.0f / child1->data) * v->grad;
                break;
            case OP_EXP:
                child1->grad += v->data * v->grad;
                break;
            case OP_RELU:
                child1->grad += (v->data > 0 ? 1.0f : 0.0f) * v->grad;
                break;
            default: break;
        }
    }
}

/* --- Model Utils --- */

float random_normal() {
    float u = ((float)rand() / (RAND_MAX)) * 2 - 1;
    float v = ((float)rand() / (RAND_MAX)) * 2 - 1;
    float r = u * u + v * v;
    if (r == 0 || r > 1) return random_normal();
    return u * sqrtf(-2 * logf(r) / r);
}

// Matrix Initialization helper
void init_matrix(Value*** mat, int rows, int cols) {
    *mat = malloc(rows * sizeof(Value*));
    for (int i = 0; i < rows; i++) {
        (*mat)[i] = malloc(cols * sizeof(Value*));
        for (int j = 0; j < cols; j++) {
            (*mat)[i][j] = new_param(random_normal() * 0.08f);
        }
    }
}

// Linear Layer
Value** linear(Value** x, int seq_len, int in_dim, Value** w, int out_dim) {
    Value** out = malloc(seq_len * sizeof(Value*)); // Array of rows
    for (int i = 0; i < seq_len; i++) {
        // x[i] is a pointer to the start of the row in the flattened input? 
        // No, x is a list of Value*. 
        // Wait, the python linear receives 'x' which is a list of Values (representing a vector of embeddings)
        // OR a list of vectors. In GPT2, x is (B, T, C). Here B=1.
        // Input x is array of pointers to Values. Length = in_dim. 
        // Actually, Python code: linear takes x (list of Values) and w (list of lists).
        // It returns a list of Values.
        
        // Let's adopt the Python signature: x is a list of `Value*`, w is matrix.
        // But wait, the loop in Python `gpt` processes the sequence. 
        // `x` in the python code changes shape. 
        // At input: x is list of Embeddings (len = n_embd).
        // Linear projects x (n_embd) -> (output_dim).
        
        // Re-implementing exactly:
        // def linear(x, w): return [sum(wi * xi for wi, xi in zip(wo, x)) for wo in w]
        // x: array of Value*, len = in_dim
        // w: array of array of Value*, dim = out_dim x in_dim
        // returns: array of Value*, len = out_dim
    }
    return NULL; // Not used directly, implemented inside logic below for clarity
}

Value** apply_linear(Value** x, int in_dim, Value** w, int out_dim) {
    Value** out = malloc(out_dim * sizeof(Value*));
    for (int o = 0; o < out_dim; o++) {
        Value* acc = new_value(0.0f);
        for (int i = 0; i < in_dim; i++) {
            acc = add(acc, mul(w[o][i], x[i]));
        }
        out[o] = acc;
    }
    return out;
}

Value** softmax(Value** logits, int len) {
    Value** out = malloc(len * sizeof(Value*));
    float max_val = -1e9;
    for (int i = 0; i < len; i++) {
        if (logits[i]->data > max_val) max_val = logits[i]->data;
    }
    // Stabilize
    Value* v_max = new_value(max_val);
    
    Value** exps = malloc(len * sizeof(Value*));
    Value* sum_exps = new_value(0.0f);
    
    for (int i = 0; i < len; i++) {
        Value* diff = sub(logits[i], v_max);
        exps[i] = vexp(diff);
        sum_exps = add(sum_exps, exps[i]);
    }
    
    for (int i = 0; i < len; i++) {
        out[i] = mul(exps[i], power(sum_exps, -1.0f));
    }
    free(exps);
    return out; // caller frees array pointer
}

Value** rmsnorm(Value** x, int len) {
    Value* sum_sq = new_value(0.0f);
    for (int i = 0; i < len; i++) {
        sum_sq = add(sum_sq, mul(x[i], x[i]));
    }
    Value* mean_sq = mul_const(sum_sq, 1.0f / len);
    Value* scale = power(add(mean_sq, new_value(1e-5f)), -0.5f);
    
    Value** out = malloc(len * sizeof(Value*));
    for (int i = 0; i < len; i++) {
        out[i] = mul(x[i], scale);
    }
    return out;
}

/* --- State Dictionary --- */
Value** wte; // [vocab_size][n_embd]
Value** wpe; // [block_size][n_embd]
Value** lm_head; // [vocab_size][n_embd]

// Layers
struct Layer {
    Value** attn_wq; // [n_embd][n_embd]
    Value** attn_wk;
    Value** attn_wv;
    Value** attn_wo;
    Value** mlp_fc1; // [4*n_embd][n_embd]
    Value** mlp_fc2; // [n_embd][4*n_embd]
} layers[N_LAYER];

int vocab_size;

void init_model() {
    init_matrix(&wte, vocab_size, N_EMBD);
    init_matrix(&wpe, BLOCK_SIZE, N_EMBD);
    init_matrix(&lm_head, vocab_size, N_EMBD);
    
    for(int i=0; i<N_LAYER; i++) {
        init_matrix(&layers[i].attn_wq, N_EMBD, N_EMBD);
        init_matrix(&layers[i].attn_wk, N_EMBD, N_EMBD);
        init_matrix(&layers[i].attn_wv, N_EMBD, N_EMBD);
        init_matrix(&layers[i].attn_wo, N_EMBD, N_EMBD);
        init_matrix(&layers[i].mlp_fc1, 4 * N_EMBD, N_EMBD);
        init_matrix(&layers[i].mlp_fc2, N_EMBD, 4 * N_EMBD);
    }
}

/* --- GPT Forward Pass --- */
// Returns logits (vocab_size)
Value** gpt(int token_id, int pos_id, Value**** keys, Value**** values) {
    // 1. Embedding
    Value** tok_emb = wte[token_id];
    Value** pos_emb = wpe[pos_id];
    Value** x = malloc(N_EMBD * sizeof(Value*));
    
    for(int i=0; i<N_EMBD; i++) {
        x[i] = add(tok_emb[i], pos_emb[i]);
    }
    
    Value** x_norm = rmsnorm(x, N_EMBD);
    // free(x); // Don't free x array yet, needed for residual? No, pointer copy.
    // Actually in C we malloc'd the array 'x'. We need to be careful with array pointers.
    // For this snippet, we leak the small pointer arrays (mallocs) inside the forward pass 
    // to keep code readable. In production, use an arena for Value** arrays too.
    
    x = x_norm;

    for (int li = 0; li < N_LAYER; li++) {
        Value** x_residual = x; // Copy pointers
        x = rmsnorm(x, N_EMBD);
        
        // Attention
        Value** q = apply_linear(x, N_EMBD, layers[li].attn_wq, N_EMBD);
        Value** k = apply_linear(x, N_EMBD, layers[li].attn_wk, N_EMBD);
        Value** v = apply_linear(x, N_EMBD, layers[li].attn_wv, N_EMBD);
        
        // Cache K, V
        // keys[li] is a list of K vectors. We assume caller handles growing this list.
        // In this C impl, keys[layer][pos][embedding_dim]
        // But Python appends k (vector).
        // Caller passes pointers to buffers.
        for(int i=0; i<N_EMBD; i++) {
            keys[li][pos_id][i] = k[i];
            values[li][pos_id][i] = v[i];
        }

        Value** x_attn = malloc(N_EMBD * sizeof(Value*));
        
        // Heads
        for (int h = 0; h < N_HEAD; h++) {
            int hs = h * HEAD_DIM;
            
            // For this head
            // q_h is slice of q
            // k_h is slice of all past k's
            
            int current_ctx_len = pos_id + 1;
            Value** attn_logits = malloc(current_ctx_len * sizeof(Value*));
            
            for (int t = 0; t < current_ctx_len; t++) {
                Value* dot = new_value(0.0f);
                for (int j = 0; j < HEAD_DIM; j++) {
                     // q[hs+j] * keys[li][t][hs+j]
                     dot = add(dot, mul(q[hs+j], keys[li][t][hs+j]));
                }
                attn_logits[t] = mul_const(dot, 1.0f / sqrtf(HEAD_DIM));
            }
            
            Value** attn_weights = softmax(attn_logits, current_ctx_len);
            
            // Weighted sum of values
            for (int j = 0; j < HEAD_DIM; j++) {
                Value* acc = new_value(0.0f);
                for (int t = 0; t < current_ctx_len; t++) {
                    acc = add(acc, mul(attn_weights[t], values[li][t][hs+j]));
                }
                x_attn[hs+j] = acc;
            }
            free(attn_logits);
            free(attn_weights);
        }
        
        // Projection
        x = apply_linear(x_attn, N_EMBD, layers[li].attn_wo, N_EMBD);
        
        // Residual
        for(int i=0; i<N_EMBD; i++) x[i] = add(x[i], x_residual[i]);
        
        // MLP
        x_residual = x;
        x = rmsnorm(x, N_EMBD);
        x = apply_linear(x, N_EMBD, layers[li].mlp_fc1, 4 * N_EMBD);
        for(int i=0; i< 4*N_EMBD; i++) x[i] = relu(x[i]);
        x = apply_linear(x, 4 * N_EMBD, layers[li].mlp_fc2, N_EMBD);
        
        for(int i=0; i<N_EMBD; i++) x[i] = add(x[i], x_residual[i]);
    }
    
    Value** logits = apply_linear(x, N_EMBD, lm_head, vocab_size);
    return logits;
}

/* --- Data & Main --- */

char* docs[MAX_DOCS];
int n_docs = 0;
char uchars[256]; // Simplistic vocab mapping
int uchar_count = 0;

void load_data() {
    FILE* f = fopen("input.txt", "r");
    if (!f) { printf("input.txt not found\n"); exit(1); }
    char line[256];
    bool char_exists[256] = {false};
    
    while(fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') line[--len] = 0;
        if (len == 0) continue;
        
        docs[n_docs] = strdup(line);
        for(int i=0; i<len; i++) char_exists[(unsigned char)line[i]] = true;
        n_docs++;
        if (n_docs >= MAX_DOCS) break;
    }
    fclose(f);
    
    // Sort chars simplistic
    for(int i=0; i<256; i++) {
        if(char_exists[i]) uchars[uchar_count++] = (char)i;
    }
    vocab_size = uchar_count + 1; // +1 for BOS
    printf("Loaded %d docs, vocab size: %d\n", n_docs, vocab_size);
}

int char_to_id(char c) {
    for(int i=0; i<uchar_count; i++) if(uchars[i] == c) return i;
    return -1;
}

int main() {
    srand(42);
    load_data();
    init_model();
    
    printf("Num params: %d\n", param_count);
    
    int BOS = uchar_count;
    
    // Training Loop
    float learning_rate = 0.01f;
    float beta1 = 0.85f;
    float beta2 = 0.99f;
    float eps = 1e-8f;
    int num_steps = 100;
    
    // Cache memory for attention (avoid malloc per step)
    // [layer][block_size][n_embd]
    Value**** cache_k = malloc(N_LAYER * sizeof(Value***));
    Value**** cache_v = malloc(N_LAYER * sizeof(Value***));
    for(int i=0; i<N_LAYER; i++) {
        cache_k[i] = malloc(BLOCK_SIZE * sizeof(Value**));
        cache_v[i] = malloc(BLOCK_SIZE * sizeof(Value**));
        for(int j=0; j<BLOCK_SIZE; j++) {
            cache_k[i][j] = malloc(N_EMBD * sizeof(Value*));
            cache_v[i][j] = malloc(N_EMBD * sizeof(Value*));
        }
    }

    for (int step = 0; step < num_steps; step++) {
        zero_tape(); // Reset graph memory
        
        char* doc = docs[step % n_docs];
        int len = strlen(doc);
        int seq_len = (len + 2); // BOS + chars + BOS
        if (seq_len > BLOCK_SIZE) seq_len = BLOCK_SIZE;
        if (seq_len < 2) continue; // safety
        
        int tokens[BLOCK_SIZE];
        tokens[0] = BOS;
        for(int i=0; i<len && i < BLOCK_SIZE-2; i++) tokens[i+1] = char_to_id(doc[i]);
        tokens[seq_len-1] = BOS;
        
        Value* loss = new_value(0.0f);
        int n_targets = 0;
        
        for (int pos = 0; pos < seq_len - 1; pos++) {
            int target = tokens[pos+1];
            
            Value** logits = gpt(tokens[pos], pos, cache_k, cache_v);
            Value** probs = softmax(logits, vocab_size);
            
            // Cross entropy: -log(prob[target])
            Value* log_prob = vlog(probs[target]);
            loss = add(loss, mul_const(log_prob, -1.0f));
            n_targets++;
            
            // Note: In C we are leaking the logits/probs arrays here strictly speaking 
            // but they point to pool nodes, so it's just the array pointers.
        }
        
        loss = mul_const(loss, 1.0f / n_targets);
        
        // Zero Gradients
        for(int i=0; i<param_count; i++) params[i]->grad = 0.0f;
        
        // Backward
        backward(loss);
        
        // Adam Update
        float lr_t = learning_rate * (1.0f - (float)step / num_steps);
        for (int i = 0; i < param_count; i++) {
            Value* p = params[i];
            adam_m[i] = beta1 * adam_m[i] + (1 - beta1) * p->grad;
            adam_v[i] = beta2 * adam_v[i] + (1 - beta2) * p->grad * p->grad;
            
            float m_hat = adam_m[i] / (1.0f - powf(beta1, step + 1));
            float v_hat = adam_v[i] / (1.0f - powf(beta2, step + 1));
            
            p->data -= lr_t * m_hat / (sqrtf(v_hat) + eps);
        }
        
        if ((step+1) % 10 == 0) {
            printf("step %4d | loss %.4f | nodes used %d\n", step+1, loss->data, node_idx);
        }
    }

    // Inference
    printf("\n--- inference ---\n");
    for (int i = 0; i < 20; i++) {
        int token = BOS;
        printf("sample %2d: ", i+1);
        
        // Clear KV cache for inference (conceptually) - actually we just overwrite
        
        for (int pos = 0; pos < BLOCK_SIZE; pos++) {
            zero_tape(); // Inference creates nodes too!
            
            Value** logits = gpt(token, pos, cache_k, cache_v);
            Value** probs = softmax(logits, vocab_size);
            
            // Simple sampling (greedy/weighted)
            float r = (float)rand() / RAND_MAX;
            float cdf = 0.0f;
            int next_tok = 0;
            for(int k=0; k<vocab_size; k++) {
                cdf += probs[k]->data;
                if (r < cdf) { next_tok = k; break; }
            }
            
            token = next_tok;
            if (token == BOS) break;
            printf("%c", uchars[token]);
        }
        printf("\n");
    }

    return 0;
}
