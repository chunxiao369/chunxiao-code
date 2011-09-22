/*
#include <string.h> 
#include <stdio.h> 
#include <sys/time.h>
int main()
{
	struct timeval m_time;
	gettimeofday(&m_time,NULL);
	printf("%d",m_time.tv_sec);
	return 0;
}
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h> 
#include <signal.h> 

/************************************************************************
 *                                                                      *
 *                               Macro                                  *
 *                                                                      *
 ************************************************************************/

typedef void Sigfunc ( int );  /* for signal handlers */

/************************************************************************
 *                                                                      *
 *                            Function Prototype                        *
 *                                                                      *
 ************************************************************************/

static void      Atexit       ( void ( *func ) ( void ) );
static void      init_signal  ( void );
static void      init_timer   ( void );
static void      on_alarm     ( int signo );
static void      on_terminate ( int signo );
static int       Setitimer    ( int which, const struct itimerval *value, 
                                struct itimerval *ovalue );
       Sigfunc * signal       ( int signo, Sigfunc *func );
static Sigfunc * Signal       ( int signo, Sigfunc *func );
static void      terminate    ( void );

/************************************************************************
 *                                                                      *
 *                            Static Global Var                         *
 *                                                                      *
 ************************************************************************/

/************************************************************************/

static void Atexit ( void ( *func ) ( void ) )
{
    if ( atexit( func ) != 0 )
    {
        perror( "atexit" );
        exit( EXIT_FAILURE );
    }
    return;
}  /* end of Atexit */

/*
 * .......
 */
static void init_signal ( void )
{
    int i;

    Atexit( terminate );
    for ( i = 1; i < 9; i++ )
    {
        Signal( i, on_terminate );
    }
    Signal( SIGTERM, on_terminate );
    Signal( SIGALRM, on_alarm );
    return;
}  /* end of init_signal */

static void init_timer ( void )
{
    struct itimerval value;

    value.it_value.tv_sec  = 1;
    value.it_value.tv_usec = 0;
    value.it_interval      = value.it_value;
    Setitimer( ITIMER_REAL, &value, NULL );
}  /* end of init_timer */

static void on_alarm ( int signo )
{
    static int count = 0;

    /*
     * ........
     */
    fprintf( stderr, "cxxu add count = %u\n", count++ );
    return;
}

static void on_terminate ( int signo )
{
    /*
     * ......atexit()..
     */
    exit( EXIT_SUCCESS );
}  /* end of on_terminate */

static int Setitimer ( int which, const struct itimerval *value, 
                       struct itimerval *ovalue )
{
    int ret;

    if ( ( ret = setitimer( which, value, ovalue ) ) < 0 )
    {
        perror( "setitimer" );
        exit( EXIT_FAILURE );
    }
    return( ret );
}  /* end of Setitimer */

Sigfunc * signal ( int signo, Sigfunc *func )
{
    struct sigaction act, oact;

    act.sa_handler = func;
    sigemptyset( &act.sa_mask );
    act.sa_flags   = 0;
    if ( signo == SIGALRM )
    {
#ifdef  SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;  /* SunOS 4.x */
#endif
    }
    else
    {
#ifdef  SA_RESTART
        act.sa_flags |= SA_RESTART;  /* SVR4, 44BSD */
#endif
    }
    if ( sigaction( signo, &act, &oact ) < 0 )
    {
        return( SIG_ERR );
    }
    return( oact.sa_handler );
}  /* end of signal */

static Sigfunc * Signal ( int signo, Sigfunc *func )
{
    Sigfunc *sigfunc;

    if ( ( sigfunc = signal( signo, func ) ) == SIG_ERR )
    {
        perror( "signal" );
        exit( EXIT_FAILURE );
    }
    return( sigfunc );
}  /* end of Signal */

static void terminate ( void )
{
    fprintf( stderr, "chunxiao end!\n" );
    return;
}  /* end of terminate */

int main ( int arg, char * argv[] )
{
    init_signal();
    init_timer();
    while ( 1 )
    {
        /*
         * .......CPU...
         */
        getchar();
    }
    return( EXIT_SUCCESS );
}  /* end of main */

/************************************************************************/

