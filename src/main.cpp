#include "robot.h"


#ifdef USE_GRAPHIC_INTERFACE
/*
 * This is GUI - starting entry.
 */
int main(int argc, char *argv[])
{
    CRobot robot( argc, argv );
    //robot.visualize();  // for example matrix
    robot.visualize_tab();
}

#else
/*
 * This is CUI - starting entry.
 */
int main (int argc, char *argv[])
{
    CRobot robot( argc, argv );
    return robot.work();
}
#endif
