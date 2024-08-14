clc
clear
%RIFF chunk
chunkID=[uint8('R'),uint8('I'),uint8('F'),uint8('F')];           %文件标识'RIFF'
chunkSize=uint32(600036);                                         %文件大小,待定
format=[uint8('W'),uint8('A'),uint8('V'),uint8('E')];            %文件格式'WAVE'
%Format chunk
subchunk1ID=[uint8('f'),uint8('m'),uint8('t'),uint8(32)];        %子块标识'fmt'
subchunk1Size=uint32(16);                                        %子块大小
audioFormat=uint16(1);                                           %音频格式1:PCM
numChannels=uint16(2);                                           %声道数，2
sampleRates=uint32(8000);                                        %采样率，8000Hz
byteRate=uint32(32000);                                          %每秒字节数byteRate=sampleRate * numChannels * (bitsPerSample / 8)
blockAlign=uint16(4);                                            %块对齐blockAlign=numChannels * (bitsPerSample / 8)
bitsPerSample=uint16(16);                                         %采样深度，16
subchunk2ID=[uint8('d'),uint8('a'),uint8('t'),uint8('a')];       %子块标识'data'
subchunk2Size=uint32(600000);                                     %子块大小subchunk2Size=采样数 * numChannels * (bitsPerSample / 8)
fileID1=fopen("a.txt");
formatSpec='%x';
data1=fscanf(fileID1,formatSpec,[600000,1]);
for i=1:600000
    if(mod(i,2)==0)
        temp1=data1(i);
        temp2=data1(i-1);
        data1(i-1)=temp1;
        data1(i)=temp2;
    end
end
fileID=fopen('sound_sample.wav','w+');%创建二进制头文件
fwrite(fileID,chunkID,"uint8")
fwrite(fileID,chunkSize,"uint32")
fwrite(fileID,format,"uint8")
fwrite(fileID,subchunk1ID,"uint8")
fwrite(fileID,subchunk1Size,"uint32")
fwrite(fileID,audioFormat,"uint16")
fwrite(fileID,numChannels,"uint16")
fwrite(fileID,sampleRates,"uint32")
fwrite(fileID,byteRate,"uint32")
fwrite(fileID,blockAlign,"uint16")
fwrite(fileID,bitsPerSample,"uint16")
fwrite(fileID,subchunk2ID,"uint8")
fwrite(fileID,subchunk2Size,"uint32")
fwrite(fileID,data1)%写入音频数据
sound_original=audioread('original_sound.mp3');%读取原始音频
sound_original_left=sound_original(:,1);%提取原始音频左声道
sound_original_left_fft=fftshift(fft(sound_original_left,910704));
sound_sample=audioread("sound_sample.wav");
sound_sample_left=sound_sample(:,1);
sound_sample_left_fft=fftshift(fft(sound_sample_left,150000));
figure(1)
t=1:1:150000;
t1=1:1:910704;
subplot(2,1,1)
plot(t1,sound_original_left)
title('原始音频时域波形（左声道）')
xlabel('采样点')
subplot(2,1,2)
plot(t,sound_sample_left)
title('采样音频时域波形（左声道）')
xlabel('采样点')
figure(2)
k=-75000:1:74999;
k=k*8000/150000;%采样点对应频率
k1=-455352:1:455351;
k1=k1*12200/910704;
subplot(2,1,1)
plot(k1,abs(sound_original_left_fft))
title('原始音频幅度谱')
xlabel('频率/Hz')
axis([-4000 4000 0 15000])
subplot(2,1,2)
plot(k,abs(sound_sample_left_fft))
title('采样音频幅度谱')
xlabel('频率/Hz')