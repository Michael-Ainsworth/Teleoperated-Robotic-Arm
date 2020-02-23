function ur5InvKinControl(gd, ur5, time)

time_int = time
theta = ur5InvKin(gd);

% Minimize change in joint angle
min = 100000; 
for i=1:8
    distance = ur5.get_current_joints - theta(:, i); 
    distance = distance(1:4);
    distance = norm(distance);
    if ( distance < min )
        min = distance;
        theta_best = theta(:, i);
    end
end
% tmp = norm(theta-Jangles);
% [~,ind] = min(tmp);
% theta_f = theta(:,ind);

% Move ur5
ur5.move_joints(theta_best,time_int);
end