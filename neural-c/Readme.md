Test Accuracy: 11.60%
Training epoch 1/5
Test Accuracy: 93.58%
Training epoch 2/5
Test Accuracy: 95.01%
Training epoch 3/5
Test Accuracy: 95.41%
Training epoch 4/5
Test Accuracy: 95.75%
Training epoch 5/5
Test Accuracy: 95.94%

对于一个MNIST点阵数据集，28*28的点阵，每个像素都是一个特征，因此输入层有784个神经元。在这种情况下，如果您使用一个具有64个神经元的隐藏层和10个输出神经元的神经网络模型，那么模型的参数数量将是784个输入神经元到隐藏层的参数（784 * 64）+ 64个隐藏层的偏置 + 64 * 10个隐藏层到输出层的参数 + 10个输出层的偏置，即大约5万个参数。
