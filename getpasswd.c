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
    puts("");
  } else {
    exit(EXIT_FAILURE);
  };
}
#endif

#ifdef __linux__
#include <stdlib.h>
#include <stdio.h>
#include <libsecret/secret.h>

static void get(const char *service, const char *username) {
  static const SecretSchema schema = {
    "org.jeff.Password", SECRET_SCHEMA_DONT_MATCH_NAME,
    {
      {  "username", SECRET_SCHEMA_ATTRIBUTE_STRING },
      {  "service", SECRET_SCHEMA_ATTRIBUTE_STRING },
      {  "NULL", 0 }
    }
  };
  GError *error = NULL;

  gchar *password = secret_password_lookup_sync(&schema,
                                                NULL, &error,
                                                "username", username,
                                                "service", service,
                                                NULL);

  if (error != NULL) {
    g_error_free(error);
    exit(EXIT_FAILURE);
  } else if (password == NULL) {
    exit(EXIT_FAILURE);
  } else {
    printf("%s\n", password);
    secret_password_free(password);
  }
}

#endif

int main(int argc, char **argv) {
  if(argc == 3) {
    get(argv[1], argv[2]);
  } else {
    puts("usage: getpasswd <site> <username>");
    return EXIT_FAILURE;
  }
}
