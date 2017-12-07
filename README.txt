# Delta
This is the code and object files for a Delta robot develop for UDOO neo that is control by a tcp session, only using three commands (home, stop, coord X Y Z )

The file DeltaK.c had the algorithm for the reverse kinematics and can be used in others plataforms.
pwm.c is exlusiv for UDOO neo in this project there were used 3 pwm moduls of the UDOO to configure them, please follow the instructions in the doc PWM_UDOO_CONF
it is very important to do the steps discribed there, in case of not doing that, pwm.c it will not work.

To init the program, execute the file Delta.out with the parameter 1500, that will start the server that will wait for a client (client program in the client directory).
