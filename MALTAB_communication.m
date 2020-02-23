clc;
tic

for i = 1:length(instrfind)
    if ~isempty(instrfind)
        fclose(instrfind);
        delete(instrfind);
    end
end 

arduino = serial('/dev/ttyACM0','BAUD', 9600); % Make sure the baud rate and COM port is
                                % same as in Arduino IDE
                                %/dev/cu.usbmodem1421                    
fopen(arduino);

pause(1)

disp("Initiating UR-5 connection...")
disp(' ')
ur5 = ur5_interface();

disp(' ')
disp("Achieving T-pose...");
joints_T = [0 -pi/2 -pi/2 -pi/2 pi/2 0];

disp(' ')
ur5.move_joints(ur5.home, 10);
pause(10);
ur5InvKinControl(ur5FwdKinNew(joints_T), ur5, 8);
pause(8)

for i = 1:35
    fscanf(arduino);
end

x_prev = -0.50;
y_prev = 0.0;
z_prev = 0.49;

% global KEY_IS_PRESSED
% KEY_IS_PRESSED = 0;
% gcf
% set(gcf, 'KeyPressFcn', @myKeyPressFcn)  

disp("T-pose achieved! Begin reading wrist position...")
disp(' ')

toc
while 1%~KEY_IS_PRESSED
    %drawnow
    tic
    readData = fscanf(arduino);
    
    [xw, yw, zw] = convertToCoords(readData);
    fprintf('%.2f, %.2f, %.2f', xw, yw, zw);
    disp(' ');
    
    g = [0 1 0 xw;
        1 0 0 yw;
        0 0 -1 zw;
        0 0 0 1];
    
    dist = abs(sqrt(xw*xw + yw*yw + zw*zw) -...
        sqrt(x_prev*x_prev + y_prev*y_prev + z_prev*z_prev));
    
    if dist > 0.001
        time = dist/0.01;
        
        if time < 0.1
            time = 0.1;
        elseif time > 5
            time = 5;
        end
        
        disp('Going to pose');
        ur5InvKinControl(g, ur5, time)

        if abs(xw - 0.0) < 0.001 && abs(yw - 0.0) < 0.001 && abs(zw - 0.0) < 0.001
            disp("Invalid reading from IMU.");
            break
        end
    end
    
    x_prev = xw;
    y_prev = yw;
    z_prev = zw;
    toc
end

disp("Closing the connection to the Arduino...");
close
fclose(arduino);
delete(arduino)

function [xw, yw, zw] = convertToCoords(readData)
    i = 1;

    num = [];
    while readData(i) ~= ','
        char = readData(i);
        num = [num, char];
        i = i + 1;
    end
    
    xw = str2double(num);
    
    i = i+1;
    num = [];
    while readData(i) ~= ','
        char = readData(i);
        num = [num, char];
        i = i + 1;
    end
    
    yw = str2double(num);
    
    i = i+1;
    num = [];
    while readData(i) ~= ',' && readData(i) ~= ' ' && ...
            i < length(readData);
        char = readData(i);
        num = [num, char];
        i = i + 1;
    end
    
    zw = str2double(num);    
end

% function myKeyPressFcn(~, ~)
%     global KEY_IS_PRESSED
%     KEY_IS_PRESSED  = 1;
%     disp('Key is pressed...')
% end