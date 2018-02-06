#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>


pthread_mutex_t lockForNumbers1 =PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lockForResource2 =PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lockForNumbers2 = PTHREAD_MUTEX_INITIALIZER;



FILE *from, *to , *from2;

//initial values
char ch = 'c';
char ch2 = 'c';

//hold the aviable condition for files initial 
//all of them avaiable and values are 1
//first element is for numbers1.txt
//second element is for numbers2.txt
//third element is for resource2.txt
int isAviable[3] = {1,1,1};

//just like boolean flag
char *isEmptyFlagForFile1 = &ch;
char *isEmptyFlagForFile2 = &ch2;


//hold the files have how many negative numbers, initial 
//all of them are 1 it means that thread have to check all files 
//at least 1 negative is there 
//at last values are 0 and threads halt when all negative numbers delete
//first element is for numbers1.txt
//second element is for numbers2.txt
//third element is for resource2.txt
int hasMoreNegative[3] ={1,1,1}; 


//hold the files have how many prime numbers, initial 
//all of them are 1 it means that thread have to check all files 
//at least 1 prime is there 
//at last values are 0 and threads halt when all prime numbers delete
//first element is for numbers1.txt
//second element is for numbers2.txt
//third element is for resource2.txt
int hasMorePrime[3] = {1,1,1};

//return 1 if value is prime otherwise return 0
int isPrime(int value){
    if (value <= 1)
		return 0;
    if (value % 2 == 0 && value > 2)
     	return 0;
    for(int i = 3; i < value / 2; i+= 2)
    {
        if (value % i == 0)
            return 0;
    }
    return 1;
}

//this function responsible for deleting prime numbers from given files
//prime numbers are copied from file and paste temp file 
//then name of the temp file is set as given file name
void deleteFromFileForThreadB(char fileName[20]){
	char line[15];
	FILE *fromInnerB = fopen(fileName,"r");
	FILE *tempForB = fopen("tempForTypeB.txt","w+");
	int hasPrime = 0;
	while(fgets(line,sizeof(line),fromInnerB)){
		if(isPrime(atoi(line))==0){
			fprintf(tempForB, "%s",line);
		}
		else
			hasPrime++;
	}
	rename("tempForTypeB.txt",fileName);
	fclose(fromInnerB);
	fclose(tempForB);
	//set global variable hasMorePrime according to given file name
	//this global variable doesn't require any mutex because
	//every file already have mutexes
	if(fileName=="numbers1.txt"){
		hasMorePrime[0] = hasPrime;
	}
	else if(fileName=="numbers2.txt"){
		hasMorePrime[1] = hasPrime;	
	}
	else if(fileName=="resource2.txt"){
		hasMorePrime[2] = hasPrime;
	}
	
}

//this function responsible for the cut line from given file
//starting number means that if the thread read 5 lines from file 
//and copy first 5 lines from this file to temp file 
//then rename temp file as given file name
void deleteFromFileForThreadA(int startingLine,char fileName[20]){
	char line[15];
	FILE *fromInnerA = fopen(fileName,"r");
	FILE *tempForA = fopen("tempForTypeA.txt","w+");
	int lineNumber = 0;
	while(fgets(line,sizeof(line),fromInnerA)){
		if(lineNumber>=startingLine){
			fprintf(tempForA, "%s",line);
		}
		lineNumber++;
	}
	
	if(!rename("tempForTypeA.txt",fileName));
	fclose(tempForA);
	fclose(fromInnerA);
}


//this function responsible for deleting negative numbers from given files
//negative numbers are copied from file and paste temp file 
//then name of the temp file is set as given file name
void deleteFromFileForThreadC(char fileName[25]){
	char line[15];
	FILE *fromInnerC = fopen(fileName,"r");
	FILE *tempForC = fopen("tempForTypeC.txt","w+");
	int hasNegative = 0;
	while(fgets(line,sizeof(line),fromInnerC)){
		if(atoi(line)>=0){
			fprintf(tempForC, "%s",line);
		}
		else
			hasNegative++;
	}
	rename("tempForTypeC.txt",fileName);
	fclose(fromInnerC);
	fclose(tempForC);
	//set global variable hasMoreNegative according to given file name
	//this global variable doesn't require any mutex because
	//every file already have mutexes
	if(fileName=="numbers1.txt"){
		hasMoreNegative[0] = hasNegative;
		
	}
	else if(fileName=="numbers2.txt"){
		hasMoreNegative[1] = hasNegative;
		
	}
	else if(fileName=="resource2.txt"){
		hasMoreNegative[2] = hasNegative;
	}
}


void *typeBrun(){
	//work until numbers1.txt don't have any prime number in it
	//this control is done by global variable hasMorePrime array  
	while(hasMorePrime[0]!=0){
		if(isAviable[0]==1){
			pthread_mutex_lock(&lockForNumbers1);
			//Type B thread hold the numbers1.txt
			isAviable[0]=0;
			deleteFromFileForThreadB("numbers1.txt");
			isAviable[0]=1;
			//TYPE B THREAD RELASE the numbers1.txt
			pthread_mutex_unlock(&lockForNumbers1);
		}
	}
	//work until numbers2.txt don't have any prime number in it
	//this control is done by global variable hasMorePrime array
	while(hasMorePrime[1]!=0){
		if(isAviable[1]==1){
			pthread_mutex_lock(&lockForNumbers2);
			//TYPE B THREAD HOLD the numbers2.txt
			isAviable[1]=0;
			deleteFromFileForThreadB("numbers2.txt");
			isAviable[1]=1;
			//TYPE B THREAD RELASE the numbers2.txt
			pthread_mutex_unlock(&lockForNumbers2);
		}
	}
	//work until resource2.txt don't have any prime number in it
	//this control is done by global variable hasMorePrime array
	while(hasMorePrime[2]!=0){
		if(isAviable[2]==1){
			pthread_mutex_lock(&lockForResource2);
			//TYPE B THREAD HOLD the resource2
			isAviable[2]=0;
			deleteFromFileForThreadB("resource2.txt");
			isAviable[2]=1;
			//TYPE B THREAD RELASE the resource2
			pthread_mutex_unlock(&lockForResource2);

		}
	}
}


void *typeCrun(){
	//work until numbers1.txt don't have any negative number in it
	//this control is done by global variable hasMorePrime array  
	while(hasMoreNegative[0]!=0){
		if(isAviable[0]==1){
			pthread_mutex_lock(&lockForNumbers1);
			//TYPE C THREAD HOLD the numbers1.txt
			isAviable[0]=0;
			deleteFromFileForThreadC("numbers1.txt");
			isAviable[0]=1;
			//TYPE C THREAD RELASE the numbers1.txt
			pthread_mutex_unlock(&lockForNumbers1);
		}
	}
	//work until numbers1.txt don't have any negative number in it
	//this control is done by global variable hasMorePrime array
	while(hasMoreNegative[1]!=0){
		if(isAviable[1]==1){
			pthread_mutex_lock(&lockForNumbers2);
			//TYPE C THREAD HOLD the numbers2.txt
			isAviable[1]=0;
			deleteFromFileForThreadC("numbers2.txt");
			isAviable[1]=1;
			//TYPE C THREAD RELASE the numbers2.txt
			pthread_mutex_unlock(&lockForNumbers2);
		}
	}
	//work until numbers1.txt don't have any negative number in it
	//this control is done by global variable hasMorePrime array
	while(hasMoreNegative[2]!=0){
		if(isAviable[2]==1){
			pthread_mutex_lock(&lockForResource2);
			//TYPE C THREAD HOLD the resource2
			isAviable[2]=0;
			deleteFromFileForThreadC("resource2.txt");
			isAviable[2]=1;
			//TYPE C THREAD RELASE the resource2
			pthread_mutex_unlock(&lockForResource2);

		}
	}

}

void *typeArun(){
  	char line[10];	
  	//work until numbers1.txt and numbers2.txt null
  	while(isEmptyFlagForFile1!=NULL || isEmptyFlagForFile2!=NULL){
  		if(isAviable[0] == 1){
  			pthread_mutex_lock(&lockForNumbers1);
  			isAviable[0]=0;
  			//TYPE A THREAD HOLD the numbers1.txt
  			if(isAviable[2] == 1){
  				pthread_mutex_lock(&lockForResource2);
	  			isAviable[2] = 0;
	  			//TYPE A THREAD HOLD the resource2
	  			int random= 1+rand()%10;
	
	  			from = fopen("numbers1.txt","r");
	  			to = fopen("resource2.txt","a");
	
		  		//cut and paste random number of line
		  		for(int i=0; i<random; i++){
			  		isEmptyFlagForFile1=fgets(line,sizeof(line),from);
			  		if(isEmptyFlagForFile1!=NULL){
			  			fprintf(to, "%s",line);
			  			}
			  			else
		  					break;
		  		}
	  			fclose(to);
	  			fclose(from);
	  			deleteFromFileForThreadA(random,"numbers1.txt");
	  			isAviable[2]=1;
	  			//TYPE A THREAD RELASE THE resource2
	  			pthread_mutex_unlock(&lockForResource2);
	  			isAviable[0]=1;
	  			//TYPE A THREAD RELASE THE NUMBERS1.TXT
  				pthread_mutex_unlock(&lockForNumbers1);
  			}else{
  				isAviable[0]=1;
  				//TYPE A THREAD RELASE THE NUMBERS1.TXT BECAUSE OF resource2 NOT AVAIBLE
  				pthread_mutex_unlock(&lockForNumbers1);
  			}
  		}else if(isAviable[1] == 1){
  			pthread_mutex_lock(&lockForNumbers2);
  			isAviable[1]=0;
  			//TYPE A THREAD HOLD the numbers2.txt
  			if(isAviable[2] == 1){
  				pthread_mutex_lock(&lockForResource2);
	  			isAviable[2] = 0;
	  			//TYPE A THREAD %d HOLD the resource2
	  			int random= 1+rand()%10;
	  		
	  			from2 = fopen("numbers2.txt","r");
	  			to = fopen("resource2.txt","a");
		  		for(int i=0; i<random; i++){
			  		isEmptyFlagForFile2=fgets(line,sizeof(line),from2);
			  		if(isEmptyFlagForFile2!=NULL){
			  			fprintf(to, "%s",line);
			  			}
			  			else
			  				break;
		  			}
	  			fclose(to);
	  			fclose(from2);
	  			deleteFromFileForThreadA(random,"numbers2.txt");
	  			isAviable[2]=1;
	  			//TYPE A THREAD RELASE THE resource2
	  			pthread_mutex_unlock(&lockForResource2);
	  			isAviable[1]=1;
	  			//TYPE A THREAD RELASE THE NUMBERS2.TXT
  				pthread_mutex_unlock(&lockForNumbers2);
  			}else{
  				isAviable[1]=1;	
  				//TYPE A THREAD RELASE THE NUMBERS2.TXT BECAUSE OF resource2 NOT AVAIBLE
  				pthread_mutex_unlock(&lockForNumbers2);
  			}
  		}
  	}
  	
}

void main(){
	
	pthread_t typeAthread;
	pthread_create(&typeAthread,NULL,(void *)typeArun,NULL);
	

	pthread_t typeAthread2;
	pthread_create(&typeAthread2,NULL,(void *)typeArun,NULL);

	pthread_t typeAthread3;
	pthread_create(&typeAthread3,NULL,(void *)typeArun,NULL);
	

	pthread_t typeAthread4;
	pthread_create(&typeAthread4,NULL,(void *)typeArun,NULL);

	pthread_t typeCthread;
	pthread_create(&typeCthread,NULL,(void *)typeCrun,NULL);

	pthread_t typeBthread;
	pthread_create(&typeBthread,NULL,(void *)typeBrun,NULL);

	pthread_join(typeAthread,NULL);
	pthread_join(typeAthread2,NULL);
	pthread_join(typeAthread3,NULL);
	pthread_join(typeAthread4,NULL);
	pthread_join(typeCthread,NULL);
	pthread_join(typeBthread,NULL);
	pthread_mutex_destroy(&lockForNumbers1);
	pthread_mutex_destroy(&lockForNumbers2);
	pthread_mutex_destroy(&lockForResource2);
	
}
