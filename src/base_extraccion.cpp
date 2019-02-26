
#include <iostream>
#include <string>
#include <sstream>
#include "twitcurl.h"
#include "oauthlib.h"
#include <algorithm>
#include "nlohmann/json.hpp"

using namespace std;
FILE _iob[] = { *stdin,*stdout,*stderr };
extern "C" FILE * __cdecl __iob_func(void) { return _iob; }
extern "C" void __imp__set_output__format(void){}

int main1() {

	twitCurl twitterObj;
	string replyMsg;
	string tmpStr, tmpStr2;
	char tmpBuf[1024];
	twitterObj.setTwitterUsername("username");
	twitterObj.setTwitterPassword("clave");
	
	// Evito poner las contrasenas por seguridad
	
	cout << "test" << endl;
	string consumerKey = "Key nuestra api";
	string consumerKeysecret = "Key Secrete de nuestra api";
	string myOAuthAccessTokenKey = "nuestro acceso mediante OAuth key";
	string myOAuthAccessTokenSecret = "nuestro acceso mediante OAuth key secret";

	// Setting Keys - Poniendo los tokens , username y password

	/* Step 0: Set OAuth related params. These are got by registering your app at twitter.com */
	twitterObj.getOAuth().setConsumerKey(consumerKey);
	twitterObj.getOAuth().setConsumerSecret(consumerKeysecret);
	twitterObj.getOAuth().setOAuthTokenKey(myOAuthAccessTokenKey);
	twitterObj.getOAuth().setOAuthTokenSecret(myOAuthAccessTokenSecret);


	// Codigo obtener tweets
	tmpStr = "petro"; // Esta es la palabra que queremos buscar aparentemente solo funciona con tweets recientes 
	replyMsg = "";
	if (twitterObj.search(tmpStr))
	{
		twitterObj.getLastWebResponse(replyMsg);
		printf("\ntwitterClient:: twitCurl::search web response:\n%s\n", replyMsg.c_str());
	}
	else
	{
		twitterObj.getLastCurlError(replyMsg);
		printf("\ntwitterClient:: twitCurl::search error:\n%s\n", replyMsg.c_str());
	}



	system("pause");
	ofstream out("Twitter_API.json");
	out << replyMsg;
	out.close();
	return 0;
}

