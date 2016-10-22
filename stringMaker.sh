n=0; 

while [[ $n -lt 100000 ]]; 
do 
  str1=`openssl rand -base64 6`; 
  str2=`openssl rand -base64 6`; 
  echo "$str1": "$str2" >> testFile2.txt;
  n=$((n+1)); 
done
