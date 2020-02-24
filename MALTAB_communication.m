clear all;  % Clear all variables
global x;  % Create global instance of x
                 
% Instrfind command shows all available ports for given computer

% For loop that cycles through all available ports. If a port is in use,
% close and delete the port.
for i = 1:length(instrfind)
    if ~isempty(instrfind)
        fclose(instrfind);
        delete(instrfind);
    end
end

% Set variable x for communcation with Serial port.
x=serial('/dev/cu.usbmodem1421','BAUD', 9600); % Make sure the baud rate and COM port is
                                               % same as in Arduino IDE
                                               %/dev/cu.usbmodem1421

fopen(x);  % Open port

instrfind  % View open ports

% While true, read all data in the serial port using fscanf command.
% Display to command window for reference.
while 1
    readData = fscanf(x);
    disp(readData)
end

fclose(x)  % Close x
delete(x)  % Delete x


