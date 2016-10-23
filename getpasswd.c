#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <Security/Security.h>
#include <CoreServices/CoreServices.h>

static void get(const char *service, const char *username) {
  uint8_t *password = NULL;
  uint32_t password_length = 0;
  OSStatus status;
  SecKeychainItemRef *item = NULL;

  status = SecKeychainFindGenericPassword (
    NULL,
    strnlen(service, 64),
    service,
    strnlen(username, 64),
    username,
    &password_length,
    (void*)&password,
    item
  );

  if (item)
    CFRelease(item);

  if(status == noErr) {
    for(uint32_t i = 0; i < password_length; i++)
      putchar(password[i]);
  } else {
    exit(EXIT_FAILURE);
  };
}
#endif

#ifdef __linux__
#include <gnome-keyring.h>

static void cb(GnomeKeyringResult res,
               const gchar* password,
               gpointer user_data) {
  if(res == GNOME_KEYRING_RESULT_OK) {
    puts(password);
  } else {
    exit(EXIT_FAILURE);
  }
}

static void get(const char *service, const char *username) {
  gnome_keyring_find_password(GNOME_KEYRING_NETWORK_PASSWORD,
                              cb,
                              NULL,
                              NULL,
                              "user", username,
                              "service", service,
                              NULL);
}

#endif

int main(int argc, char **argv) {
  if(argc == 3) {
    get(argv[1], argv[2]);
  } else {
    printf("usage: getpasswd <site> <username>");
    return EXIT_FAILURE;
  }
}
