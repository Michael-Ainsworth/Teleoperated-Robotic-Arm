function gst = ur5FwdKin(q)
%% Vectors
e2 = [0 1 0];
e3 = [0 0 1];
zeroVec = [0 0 0];
%% Lengths (m)
L0 = 89.2/1000;
L1 = 425/1000;
L2 = 392/1000;
L3 = 109.3/1000;
L4 = 94.75/1000;
L5 = 82.5/1000;

%% Define q+w
w1 = e3; q1 = zeroVec;
w2 = e2; q2 = [0 0 L0];
w3 = e2; q3 = [L1,0,L0];
w4 = e2; q4 = [L1+L2,0,L0];
w5 = -e3; q5 = [L1+L2,L3,0];
w6 = e2; q6 = [L1+L2,L3,-L4+L0];

%% Define gst0
gst0 = [-1 0 0 L1+L2;...
    0 0 1 L3+L5;...
    0 1 0 -L4+L0;...
    0 0 0 1];

%% Calculate Xi
twists = [[cross(q1,w1), w1]',...
    [cross(q2,w2), w2]',...
    [cross(q3,w3), w3]',...
    [cross(q4,w4), w4]',...
    [cross(q5,w5), w5]',...
    [cross(q6,w6), w6]'];

%% Compute Forward Kinematics
gst = twistExp(twists(:,1),q(1));
for i = 2:6
    gst = gst*twistExp(twists(:,i),q(i));
end
gst = gst*gst0;
end