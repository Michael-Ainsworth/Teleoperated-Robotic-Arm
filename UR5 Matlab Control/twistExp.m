function g = twistExp(twist,theta)
    w = twist(4:6);
    v = twist(1:3);
    if norm(w) == 0
        R = eye(3);
        p = v * theta;
    else
        R = eye(3) + sin(theta)*SKEW3(w) + (1 - cos(theta))*...
            SKEW3(w)*SKEW3(w);
        p = (eye(3) - R)*(SKEW3(w)*v) + w*(w'*v)*theta;
    end
    g = [[R p]; [0 0 0 1]];
end