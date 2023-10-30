
#include <stdint.h> // typedef uint8_t
#ifndef UINT8_MAX
#error "No support for uint8_t"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
int main( int argc, char* argv[] ) {

	static char const alphabet[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ"
					"abcdefghijklmnopqrstuvwxyz"
					"0123456789+/=";
	int input_fd = STDIN_FILENO;
	int output_fd = STDOUT_FILENO;
	size_t count = 0;
/**Selecting input source**/

	if(argc > 2) {
		
		fprintf(stderr, "more than one argument received.\n");
		return 1;
	}
	else if ( argc == 2) {
		if(strncmp(argv[1], "-", 1) == 0){
			input_fd = 0;
		}else{	
		input_fd = open(argv[1], O_RDONLY);
		}		
		if (input_fd == -1){
			fprintf(stderr, "not a valid file");
			return 1;
		}
	} else if (argc == 1){
			input_fd = 0;
		}
	else{
		fprintf(stderr, "Incorrect argument");
		return 1;
	}	


/***Reading the input*****/
	while (1){	
	uint8_t in[3], out[4];

	size_t nread =0;

	for (size_t offset =0; offset < sizeof in;)
	{
		fflush(NULL);
		ssize_t n = read(input_fd, offset + (char *) in, sizeof in - offset);

		if ( n < 0){
			fprintf(stderr, "Read failed");
			break;
		}		
		if (n==0){
			break;
		}			
		offset += n;
		nread = offset / sizeof *in;
		}
	if (nread == 1) {
		out[0] = alphabet[(in[0] >> 2)];
        	out[1] = alphabet[((in[0] & 0x03) << 4)];
        	out[2] = alphabet[64];
        	out[3] = alphabet[64]; 
		
		} else if(nread ==2){
		out[0] = alphabet[(in[0] >> 2)];
        	out[1] = alphabet[(((in[0] & 0x03) << 4) | (in[1] >> 4))];
        	out[2] = alphabet[((in[1] & 0x0F) << 2)];
        	out[3] = alphabet[64]; 
		
		}else{
        	out[0] = alphabet[(in[0] >> 2)];
        	out[1] = alphabet[(((in[0] & 0x03) << 4) | (in[1] >> 4))];
        	out[2] = alphabet[(((in[1] & 0x0F) << 2) | (in[2] >> 6))];
        	out[3] = alphabet[(in[2] & 0x3f)];
			}	

	for (size_t offset =0; offset < sizeof out;)
	{
		ssize_t n = write(output_fd, offset + (char *) out, sizeof out - offset);
		if (n < 0) {
			fprintf(stderr, "Write failed");
			break;
		}
		count += 4;
		offset += n;
		}
		if (nread < sizeof in / sizeof *in)
		{
		write(STDOUT_FILENO,"\n",1);
		break;
		}
		if (count == 76) {
		write(STDOUT_FILENO,"\n",1);
		count =0;
}	

}
	


return 0;

}

