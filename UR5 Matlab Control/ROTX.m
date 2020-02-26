function R_X = ROTX(roll)

    R_X = [1 0 0; 0 cos(roll) -sin(roll); 0 sin(roll) cos(roll)];
    
end