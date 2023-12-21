q = load('input_q2.txt');
X = load('interpolationTable_Lobith.txt');
X(:,3) = log10(X(:,2));

interpQ = interp1(X(:,3),X(:,1),log10(q));

csvwrite('interpolationResults.csv',[q interpQ]);