clear all

data = importdata('test.csv')

data.textdata;
data.data;
data.rowheaders;

% Reads timestamp, splits it up, converts to integer for each row
for ii = 1:size(data.textdata)
    tmp1 = strsplit(data.textdata{ii},'/');
    tmp1 = str2double(tmp1);
    sdate(ii) = datenum(tmp1(1),tmp1(2),tmp1(3),tmp1(4),tmp1(5),tmp1(6));
    
    % For some reason, the first year value isn't recorded properly
    % Manually sets this to be 2021
    if (ii == 1) 
       sdate(ii) =  datenum(2021,tmp1(2),tmp1(3),tmp1(4),tmp1(5),tmp1(6));
    end
    
end

sdate = sdate';
food = data.data(:,1);
water = data.data(:,2);
daylight = data.data(:,4);

tiledlayout(3,1)

nexttile
plot(sdate,food)
datetick('x','HHPM')
title('Food Level (g)')

nexttile
plot(sdate,water)
title('Water Level (mL)')
datetick('x','HHPM')

nexttile
plot(sdate,daylight)
datetick('x','HHPM')
title('Daylight (Y/N)')
