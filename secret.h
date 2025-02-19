#include <pgmspace.h>
 
#define SECRET
#define THINGNAME "ESP32_TEST"                    //change this
 
//const char WIFI_SSID[] = "Moonflower Main";               //change this
//const char WIFI_PASSWORD[] = "squeebly4836now";           //change this
const char WIFI_SSID[] = "Precinct Seven";               //change this
const char WIFI_PASSWORD[] = "Versailles7666x";  
const char AWS_IOT_ENDPOINT[] = "a117pwb9ukyk3o-ats.iot.us-east-2.amazonaws.com";       //change this
 
// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";
 
// Device Certificate                                               //change this
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVAORHqS969Oa5v3iKQplWqRhVZYk9MA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNDA4MTUwMTMx
NTdaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDLaS+H7rjo/l1lk47i
Od4m5o5R2L2edt3YNXmNdk5hqJTlUUKPwLEguX5rL4EzZZrJHvNe+Uqz7FfjDEVG
t/gOBy3Nm/Go6gGpM/10CM359IfE06xmAciLvfVtILULjevEzKe6HMuh2wcLisy6
J694ZULoEoQNw7110S6cWxXtCfp/pIo+wBjSCJUlrCQXQHmZjzcCOZ12qkfuI9Mr
aVmR+DEI3/Pl1TeMHNP7S7b7m5yZe09Ys+BPnk9smPPNgvnUokDf5GYzl3TK3Z65
IQDXKJ2jcWpehd/n/OAcKGEEi9turRzmPQeR+rcibpKTyeHwP+OO6NmRBy3m1Fzc
tySLAgMBAAGjYDBeMB8GA1UdIwQYMBaAFNDUZwo3beM6UMFIWdR5CVXOadPmMB0G
A1UdDgQWBBTuXP+D2nM0cnkz0kfVjbxO+9glqzAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAPsz33GxFJ/x4coxBxscsRKBb
wMtU8cs2O+Hjfb2TFhwLln9f81MIj7jCUzw+4F3iSWERz9/wPOkwChRnBx6URYUJ
3OyC7DmR7VBkXQJoRwVdk5DZGJIsojE2c9BDZu9lCDzAzFd2AK59YBJG8rb1dHdw
FTBKTsICYMyi5cyK3/wlNy2/WxS5TYYUPV4jFS/kpKwbH1M9zv36iMVrYfKRhJCs
yNaqytU2M+DJi2jjZPJy2rsxl4BIx1xCof5DBb8SH43yGzIRo2hGfNALeomm5ALi
S+ZSRXsnJ/bBdyHW3ZA2z7rSxqTk87RwV41E/Xvxdwka1kkzZ3qdorQF1GbFNQ==
-----END CERTIFICATE-----
)KEY";
 
// Device Private Key                                               //change this
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEogIBAAKCAQEAy2kvh+646P5dZZOO4jneJuaOUdi9nnbd2DV5jXZOYaiU5VFC
j8CxILl+ay+BM2WayR7zXvlKs+xX4wxFRrf4DgctzZvxqOoBqTP9dAjN+fSHxNOs
ZgHIi731bSC1C43rxMynuhzLodsHC4rMuieveGVC6BKEDcO9ddEunFsV7Qn6f6SK
PsAY0giVJawkF0B5mY83AjmddqpH7iPTK2lZkfgxCN/z5dU3jBzT+0u2+5ucmXtP
WLPgT55PbJjzzYL51KJA3+RmM5d0yt2euSEA1yido3FqXoXf5/zgHChhBIvbbq0c
5j0Hkfq3Im6Sk8nh8D/jjujZkQct5tRc3LckiwIDAQABAoIBACKkGjUjIZiEYTGA
TLAYPM4RK6GisMBA7Az7JmVCBZ06EYlwKk7C+Sdij7X/VBiFsDOaMsY03FUB5gHD
wDSH9hWlgVZnvWMP3xKKyRouua1cyJZrmguROC8cPaTFEjASjllopRPb/Y3l+DAu
BimZwpdVX2yYYqbZc/WRl0eHd5dzcsygGYVOzg8v2e7uuLN/o/fgJ0lGrg/s1tWL
kFVPxamrbRqd870EwSFZJQ8YPoFJ3tEeAGlmyZtG8Rjj8ujn6bKlGQlGA90hZ4o0
FJSSAJHl82BmvTO0n/ScNKXrKlKygJUA0mpbRq/uKx9XJwzYPWKMykKnYrj3Wxd4
wsoLsFECgYEA7a4QthafPrfBkiz0p5z1Lbnvn7/fLqfX2fr7s1B3LQgKxlewqPF6
P1iO8LN2gbNmkuxpwCkeWuE6/DM8mYQ03A2A1aF3KXesUR0Y4xNiIXX9gHnRmVjz
e+0zgFXQqZZmt8eISr3SeHk4fPtIbHcCK2uyRLSbJhgp834uBiV10JMCgYEA2xbr
Gci/rq7di2IHSuIB6QDPesjVb7BFq6odvJwSvsCJ1FbJ5y3evUA7a39+eDM2EUqa
qiaK1RcIARcpHbCp09ysT1IsntryNibuNUR6s1fAlo9kpem1FD7EqoYN8g93EFnt
t3SBYVsG7BwKi/PsM1KpQ5OBTPtCYQnmfNGTbykCgYAXgRwz9Z6/5Aone3yiSrpQ
UE/rG11lzsFg1BoqDG4MrPvry2Yz/tVZCFfyY7z3bJdbNjfrAeX3lMhxgluezt0v
WcKA3dvb5PLMfrUkV5lft7xfBSz+HjbN7ZInQdVFwBLeX8QRqB31Pf+hFHSADXDQ
fHD/rGCfbspPQw4oG3JAMwKBgF2iQN4nLszQTTzpq2Y0i05ymznFpXpzYoNIVCcs
qNI/ZWiLowSbVQua7Qo3Vey3Lk9bjBhWGpWRn0kgBeF7hElYkYfg/dLyjfvYzYuU
dcA6K8PtCr3h0RMS9GhQnJssQt2JIZB7SWQ7TA0SuusvV25j0JVh8CvfYnIokgoc
Y59pAoGACSFcTbIMVdfLma0VaFyL0UR5sOul2T5SZIzm2kIC64QTM2wnAIA0mjfy
mwDu7/IEz6OE6UJOiFeeg8RD2WIoa7mIKHiyktrT0Y4up02W8SBaBNqrembD7pqY
vsY9IhwZng7PakkjQgWeoiwWcKx+MVa9rHGa+MI0fggcorU2U50=
-----END RSA PRIVATE KEY-----

 
 
)KEY";