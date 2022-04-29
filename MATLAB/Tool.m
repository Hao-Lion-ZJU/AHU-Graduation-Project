function varargout = Tool(varargin)
% TOOL MATLAB code for Tool.fig
%      TOOL, by itself, creates a new TOOL or raises the existing
%      singleton*.
%
%      H = TOOL returns the handle to a new TOOL or the handle to
%      the existing singleton*.
%
%      TOOL('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in TOOL.M with the given input arguments.
%
%      TOOL('Property','Value',...) creates a new TOOL or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before Tool_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to Tool_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help Tool

% Last Modified by GUIDE v2.5 14-Apr-2022 17:27:05

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @Tool_OpeningFcn, ...
                   'gui_OutputFcn',  @Tool_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before Tool is made visible.
function Tool_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to Tool (see VARARGIN)

%这里初始化一些全局变量
global N;
global count;
global baud;
global port;
global isoffset;
global offsetbuf
global step_port;
global dis_X;
global dis_Y;
global dis_step;
global auto_flag;   %自动采集标志
offsetbuf=zeros(1,16);
baud=115200;  %初始化波特率
port=' ';
step_port=' ';
N=0;        %样本点数
count=0;    %初始化采样次数
isoffset=0;
dis_X=0;
dis_Y=0;
dis_step=0;
auto_flag=0;

% Choose default command line output for Tool
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes Tool wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = Tool_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on selection change in Buad.
function Buad_Callback(hObject, eventdata, handles)
% hObject    handle to Buad (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns Buad contents as cell array
%        contents{get(hObject,'Value')} returns selected item from Buad
global buad;
vall=get(handles.Buad,'Value');
switch vall
    case 1
        buad=9600;
    case 2
        buad=19200;
    case 3
        buad=38400;
    case 4
        buad=57600;
    case 5
        buad=115200;
    case 6
        buad=128000;
    case 7
        buad=460800;
end
% --- Executes during object creation, after setting all properties.
function Buad_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Buad (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in pport.
function pport_Callback(hObject, eventdata, handles)
% hObject    handle to pport (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns pport contents as cell array
%        contents{get(hObject,'Value')} returns selected item from pport
global port
freeports=serialportlist("available");
if isempty(freeports)
    msgbox('没有串口连接','Error','error')
else
set(handles.pport,'String',freeports);
end
vall=get(handles.pport,'Value');
port=freeports(vall);


% --- Executes during object creation, after setting all properties.
function pport_CreateFcn(hObject, eventdata, handles)
% hObject    handle to pport (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in open_Serial.
function open_Serial_Callback(hObject, eventdata, handles)
% hObject    handle to open_Serial (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global port;
global buad;
if port==' '
    msgbox('请选择串口号','Error','error')
    return
end
display(port)
Conm_num_str = port;
Buadrate_Double=buad;
global Scom;
Scom=serial(Conm_num_str,'BaudRate',Buadrate_Double);
Scom.BytesAvailableFcnMode='byte';
Scom.BytesAvailableFcnCount=32; %
Scom.BytesAvailableFcn={@mycallback,handles};

%串口事件回调设置
try
    fopen(Scom);
catch
     msgbox('串口打开失败','Error','error')
end
set(handles.collect,'enable','on');
set(handles.open_Serial,'enable','off');%打开串口的按钮变成灰色，不再可用
set(handles.Buad,'enable','off');
set(handles.pport,'enable','off');                                                                                           

%---自定义串口接收32个字节进入中断函数
function mycallback(hObject, eventdata,handles)
% hObject    对象句柄
% event      定义中断事件
% handles    structure with handles and user data (see GUIDATA)
global Scom;
global auto_flag
global Samples;
global count;
global isoffset;
global offsetbuf;
 %这里是保证fscanf不会出现因终止符问题的回传错误
if Scom.BytesAvailable == 0
     return
end
databuf=zeros(1,16);
recives=fread(Scom,32,'uint8');
j=0;
for i=1:2:32
     j=j+1;
     databuf(j)=(recives(i)*256+recives(i+1)); %将stm32发送的两段 uint8 转换
     tmp=fix(databuf(j)/32768);  %提取最高符号位
     if tmp==0
         databuf(j)=databuf(j)/32767.0*5.0;
     else
         anti=mod(databuf(j),32768);
         databuf(j)=-anti/32767.0*5.0;
     end
end 
if isoffset==1
    offsetbuf=databuf;
    disp("地磁偏移量为:");
    disp(offsetbuf);
    isoffset=0;
    return;
end
databuf=databuf-offsetbuf;
%将数据打印在表格内
set(handles.U1,'string',num2str(databuf(1)));
set(handles.U2,'string',num2str(databuf(2)));
set(handles.U3,'string',num2str(databuf(3)));
set(handles.U4,'string',num2str(databuf(4)));
set(handles.U5,'string',num2str(databuf(5)));
set(handles.U6,'string',num2str(databuf(6)));
set(handles.U7,'string',num2str(databuf(7)));
set(handles.U8,'string',num2str(databuf(8)));
set(handles.U9,'string',num2str(databuf(9)));
set(handles.U10,'string',num2str(databuf(10)));
set(handles.U11,'string',num2str(databuf(11)));
set(handles.U12,'string',num2str(databuf(12)));
set(handles.U13,'string',num2str(databuf(13)));
set(handles.U14,'string',num2str(databuf(14)));
set(handles.U15,'string',num2str(databuf(15)));
set(handles.U16,'string',num2str(databuf(16)));
set(handles.offset,'enable','on');

count=count+1;
set(handles.count_sample,'String','采集帧数：'+string(count));
 %刷新串口缓存
fclose(Scom);
fopen(Scom);

global x;
global y;
global Tcom
global dis_step;
if auto_flag==1
    count_x=mod((count-1),x)+1;
    count_y=fix((count-1)/x)+1;
    Samples(count_y,count_x)=databuf(1);    %这里就选用第一个传感器
    if count_x==x
        fprintf(Tcom,'l'+string(dis_step*(x-1))+' ');%如果x方向采集满，则换行重新开始
        pause(0.5);
        fprintf(Tcom,'f'+string(dis_step)+' ');
    else
        fprintf(Tcom,'r'+string(dis_step)+' ');%x方向未采集满，则只需又移一个步进距离
    end
    if count==x*y
        auto_flag=0;
        save('C:/Users/xcbns/Desktop/line','Samples');
        msgbox('采集完成','Success');
        set(handles.auto_collect,'enable','on');
        set(handles.collect,'enable','on');
    end
end
%      if count==N*N
%          msgbox('采集完成','Success');
%      end
%      count_x=mod((count-1),N)+1;
%      count_y=fix((count-1)/N)+1;
%      if count_x==N
%          msgbox('x方向采集完成，请回到原点采集y方向','Success');
%      end
%      for m=1:16
%          tmp_a=fix((m-1)/4)*N;
%          tmp_b=mod((m-1),4)*N;
%          Samples(tmp_a+count_y,tmp_b+count_x)=databuf(m);    %将一帧数据保存在数组内 
%      end 




% --- Executes on button press in collect.
function collect_Callback(hObject, eventdata, handles)
% hObject    handle to collect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global Scom;
fprintf(Scom,'c');%串口发送采集命令



% --- Executes on button press in close_Serial.
function close_Serial_Callback(hObject, eventdata, handles)
% hObject    handle to close_Serial (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.open_Serial,'enable','on');%打开串口的按钮重新可用
set(handles.Buad,'enable','on');
set(handles.pport,'enable','on');
global Scom;                            %全局变量Scom
fclose(Scom);
delete(Scom);
 msgbox('串口已关闭','Success')

% --- Executes on button press in auto_collect.
function auto_collect_Callback(hObject, eventdata, handles)
% hObject    handle to auto_collect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global auto_flag
global dis_step
global Samples;
global count;
global x;
global y;
global dis_X
global dis_Y
global Scom;
x=fix(dis_X/dis_step);
y=fix(dis_Y/dis_step);
if x==0||y==0||dis_step==0
    msgbox('采样面积不能为0或者无穷','Error','error');
else
    Samples=zeros(y,x);
    count=0;
    auto_flag=1;    %开始自动采集
    fprintf(Scom,'c');%串口发送采集命令
    set(handles.auto_collect,'enable','off');
    set(handles.collect,'enable','off');
end


% --- Executes on button press in close.
function close_Callback(hObject, eventdata, handles)
% hObject    handle to close (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
scoms = instrfind; %读取所有存在的端口
if ~isempty(scoms)
    stopasync(scoms); fclose(scoms); delete(scoms);%停止并且删除串口对象
end
clear 
clc
close all



% --- Executes on button press in show.
function show_Callback(hObject, eventdata, handles)
% hObject    handle to show (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

%显示样本图片
load C:\Users\xcbns\Desktop\samples.mat 
load C:\Users\xcbns\Desktop\inv.mat 
x=0:0.5:120;
y=0:0.5:30;
c=(0:239)*0.5;
d=(0:59)*0.5;
[A,B]=meshgrid(c,d);
axes(handles.Data);
surf(A,B,B_y)
axes(handles.current);
imagesc(x,y,abs(F_J_x))


% --- Executes on button press in offset.
function offset_Callback(hObject, eventdata, handles)
% hObject    handle to offset (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global isoffset;
global Scom;
isoffset=1;
fprintf(Scom,'c');%串口发送采集命令
set(handles.offset,'enable','off');


% --- Executes on selection change in sport.
function sport_Callback(hObject, eventdata, handles)
% hObject    handle to sport (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns sport contents as cell array
%        contents{get(hObject,'Value')} returns selected item from sport
global step_port
freeports=serialportlist("available");
if isempty(freeports)
    msgbox('没有串口连接','Error','error')
else
set(handles.sport,'String',freeports);
end
vall=get(handles.sport,'Value');
step_port=freeports(vall);

% --- Executes during object creation, after setting all properties.
function sport_CreateFcn(hObject, eventdata, handles)
% hObject    handle to sport (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- 步进电机串口打开按钮的事件回调
function stepper_open_Callback(hObject, eventdata, handles)
% hObject    handle to stepper_open (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global step_port;
if step_port==' '
    msgbox('请选择串口号','Error','error')
    return
end
display(step_port)
Conm_num_str = step_port;
Buadrate_Double=115200;     %与步进电机的通信默认波特率为115200
global Tcom;
Tcom=serial(Conm_num_str,'BaudRate',Buadrate_Double);
Tcom.BytesAvailableFcnMode='byte';
Tcom.BytesAvailableFcnCount=1; %接收步进电机已经移到成功信息
Tcom.BytesAvailableFcn={@step_callback,handles};
%串口事件回调设置
try
    fopen(Tcom);
catch
     msgbox('串口打开失败','Error','error')
end
set(handles.auto_collect,'enable','on');
set(handles.stepper_open,'enable','off');%打开串口的按钮变成灰色，不再可用
set(handles.sport,'enable','off');     


%---步进电机移动后，串口接收1个字节进入中断函数，然后发送采集命令
function step_callback(hObject, eventdata,handles)
% hObject    对象句柄
% event      定义中断事件
% handles    structure with handles and user data (see GUIDATA)
global Scom;
global Tcom;
ch=fread(Tcom,1,'uint8');
if ch=='y'
    fprintf(Scom,'c');%串口发送采集命令
end
 %刷新串口缓存
fclose(Tcom);
fopen(Tcom);


% --- Executes on button press in Front.
function Front_Callback(hObject, eventdata, handles)
% hObject    handle to Front (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global dis_step;
global Tcom;
fprintf(Tcom,'f'+string(dis_step)+' ');


% --- Executes on button press in Left.
function Left_Callback(hObject, eventdata, handles)
% hObject    handle to Left (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global dis_step;
global Tcom;
fprintf(Tcom,'l'+string(dis_step)+' ');

% --- Executes on button press in Right.
function Right_Callback(hObject, eventdata, handles)
% hObject    handle to Right (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global dis_step;
global Tcom;
fprintf(Tcom,'r'+string(dis_step)+' ');

% --- Executes on button press in Back.
function Back_Callback(hObject, eventdata, handles)
% hObject    handle to Back (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global dis_step;
global Tcom;
fprintf(Tcom,'b'+string(dis_step)+' ');


function Sample_Y_Callback(hObject, eventdata, handles)
% hObject    handle to Sample_Y (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Sample_Y as text
%        str2double(get(hObject,'String')) returns contents of Sample_Y as a double
global dis_Y
dis_Y=str2double(get(handles.Sample_Y,'String'));
if dis_Y<0
    dis_Y=0;
    msgbox('请输入正数','Error','error')
end
set(handles.Sample_Y,'String',num2str(dis_Y));

% --- Executes during object creation, after setting all properties.
function Sample_Y_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Sample_Y (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function Sample_X_Callback(hObject, eventdata, handles)
% hObject    handle to Sample_X (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of Sample_X as text
%        str2double(get(hObject,'String')) returns contents of Sample_X as a double
global dis_X
dis_X=str2double(get(handles.Sample_X,'String'));
if dis_X<0
    dis_X=0;
    msgbox('请输入正数','Error','error')
end
set(handles.Sample_X,'String',num2str(dis_X));


% --- Executes during object creation, after setting all properties.
function Sample_X_CreateFcn(hObject, eventdata, handles)
% hObject    handle to Sample_X (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in stepper_close.
function stepper_close_Callback(hObject, eventdata, handles)
% hObject    handle to stepper_close (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.stepper_open,'enable','on');%打开串口的按钮重新可用
set(handles.sport,'enable','on');
set(handles.auto_collect,'enable','off');
global Tcom;                            %全局变量Tcom
fclose(Tcom);
delete(Tcom);
 msgbox('串口已关闭','Success')



function step_Callback(hObject, eventdata, handles)
% hObject    handle to step (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of step as text
%        str2double(get(hObject,'String')) returns contents of step as a double
global dis_step
dis_step=str2double(get(handles.step,'String'));
if dis_step>9
    dis_step=9;
    msgbox('输入步进距离太大','Error','error')
end
if dis_step<0
    dis_step=0;
    msgbox('请输入正数','Error','error')
end
set(handles.step,'String',num2str(dis_step));



% --- Executes during object creation, after setting all properties.
function step_CreateFcn(hObject, eventdata, handles)
% hObject    handle to step (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
