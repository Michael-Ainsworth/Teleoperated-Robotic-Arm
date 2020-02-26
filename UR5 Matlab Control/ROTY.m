function R_Y = ROTX(pitch)

    R_Y=[cos(pitch) 0 sin(pitch); 0 1 0; -sin(pitch) 0 cos(pitch)];

end