clc;

ur5 = ur5_interface();

g_t_pose = [1 0 0 -0.2;
    0 1 0 -0.25;
    0 0 -1 0.45;
    0 0 0 1];

g_out = [1 0 0 -0.6;
    0 1 0 -0.6;
    0 0 -1 0.45;
    0 0 0 1];

disp('Going home');
ur5.move_joints(ur5.home,10);
pause(10);

disp('Going to T-pose 1');
ur5InvKinControl(g_out, ur5);

disp('Going to T-pose 2');
ur5InvKinControl(g_t_pose, ur5)