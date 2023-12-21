clear all
interpolationTable = load('interpolation_Lobith_returnPeriod.txt');

inputT = [linspace(1,2,10) linspace(2,25,10) linspace(25,10000,20)]';

outputQ = interp1(interpolationTable(:,1),interpolationTable(:,2),inputT);

csvwrite('..\..\unitTests\benchmarkTables\interpolationResults_ReturnPeriod.csv',[inputT outputQ]);