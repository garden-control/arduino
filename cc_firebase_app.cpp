#include "cc_firebase_app.h"

cc::FirebaseApp cc::FirebaseApp::unico;

cc::FirebaseApp::FirebaseApp()
    :asyncClient(sslClient, getNetwork(defaultNetwork))
{
    
}

void cc::FirebaseApp::iniciar()
{

}