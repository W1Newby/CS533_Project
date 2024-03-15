#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define DEV_PATH "/dev/mydevice"
int menu();
int myopen(int devptr);
int myread(int devptr);
int mywrite(int devptr);
void myseek(int devptr);
void end(int devptr);
void cleanup();

int main() {
	int option = 0;
	int devptr =-1;
	while(option !=5){	
		option = menu();
		switch(option){
			case 1: 
				//printf("function offline\n");
				devptr = myopen(devptr);
				break;
			case 2:
				myread(devptr);
				break;
			case 3:
				mywrite(devptr);
				break;
			case 4:
				myseek(devptr);
				break;
			case 5:
				end(devptr);
				break;
			default:
				printf("Invalid, try again\n");
		}
	}


	exit(EXIT_SUCCESS);
}

int menu(){

	int option =0;
	printf("Please select the number for the operation you would like to perform:\n");
	printf("1. open device.\n");
	printf("2. read from device.\n");
	printf("3. write to device.\n");
	printf("4. INCOMPLETE - Seek within the device.\n");
	printf("5. close device and exit program.\n");

	scanf("%d", &option);
	cleanup();
	return option;
}

int myopen(int devptr){
	devptr = open(DEV_PATH, O_RDWR);
	if(devptr<0){
		perror("Failed to open the device file.\n");
		printf("devptr is: %d", devptr);
		return 1;
	}
	else{
		printf("File opened successfully.\n");
		printf("devptr is: %d\n", devptr);
		return devptr;

	}
}

	int myread(int devptr){
		//int devptr = open(DEV_PATH, O_RDONLY);
		char readbuff[900];

		ssize_t bytes_read = read(devptr, readbuff, sizeof(readbuff));
		if(bytes_read <0){
			perror("Failed to read from the device.\n");
			return devptr;
		}
		else{
			printf("Data read from the device:\n%s\n", readbuff);
		}
		return devptr;
	}

	int mywrite(int devptr){
		//int devptr = open(DEV_PATH, O_RDWR);
		char towrite[900];

		printf("Please enter data to be written to device: \n");
		fgets(towrite, sizeof(towrite), stdin);
		ssize_t bytes_written = write(devptr, towrite, strlen(towrite));
		if(bytes_written<0){
			perror("Failed to write to device.\n");
		}
		else{
			printf("Data successfully written to the device.\n");
		}
		return devptr;
	}

	void myseek(int devptr){
		off_t offset;
		int whence;
		printf("Please enter desired offset: ");
		scanf("%ld", &offset);
		cleanup();
		printf("Please enter whence value (0 for SEEK_SET, 1 for SEEK_CUR, 2 for SEEK_END_: ");
		scanf("%d", &whence);
		cleanup();
		off_t position = lseek(devptr, offset, whence);
		if(position <0){
			perror("Failed to seek within the device.\n");
		}
		else{
			printf("Successfully perfomed seek within device.\n");
		}
	}

	void end(int devptr){
		
		if(devptr !=-1){
			close(devptr);
		}
		return;
	}

	void cleanup(){
		int clean;
		while((clean=getchar()) != '\n' && clean!=EOF){};
	}
