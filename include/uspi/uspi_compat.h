/*
 * uspi_compat.h
 */

int USPiInitialize (void);
int USPiKeyboardAvailable (void);
typedef void TUSPiKeyPressedHandler (const char *pString);
void USPiKeyboardRegisterKeyPressedHandler (TUSPiKeyPressedHandler *pKeyPressedHandler);
