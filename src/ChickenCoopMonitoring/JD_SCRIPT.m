clear all

%data = csvread('test.csv', 0, 1);
data = importdata('test.csv')
% Timestamp, Food, Water, Temperature, Daylight

data.textdata;
data.data;
data.rowheaders;

for ii = 1:size(data.textdata)
    tmp1 = strsplit(data.textdata{ii},'/');
    tmp1 = str2double(tmp1);
    sdate(ii) = datenum(tmp1(1),tmp1(2),tmp1(3),tmp1(4),tmp1(5),tmp1(6));
    
    if (ii == 1) 
       sdate(ii) =  datenum(2021,tmp1(2),tmp1(3),tmp1(4),tmp1(5),tmp1(6));
    end
    
end

sdate = sdate';
food = data.data(:,1);
water = data.data(:,2);

tiledlayout(2,1)

nexttile
plot(sdate,food)
datetick('x','HHPM')
title('Food Level (g)')

nexttile
plot(sdate,water)
title('Water Level (mL)')
datetick('x','HHPM')