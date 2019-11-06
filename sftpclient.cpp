#include "sftpclient.h"

#include <iostream>
#include <fstream>
using namespace std;

struct SftpFile
{
	string filename;
	FILE *stream;
};

size_t writeCallback(void *buffer, size_t size, size_t nmemb, void *stream)
{
	SftpFile *sftp_file = reinterpret_cast<SftpFile *>(stream);
	if(!sftp_file->stream)
	{
		sftp_file->stream = fopen(sftp_file->filename.c_str(), "wb");
		if(!sftp_file->stream)
			return -1;
	}
	return fwrite(buffer, size, nmemb, sftp_file->stream);
}

SftpClient::SftpClient(const std::string &ip, const std::string &username)
	: _ip{ip}, _username{username}
{
	curl_global_init(CURL_GLOBAL_DEFAULT);
}

SftpClient::~SftpClient()
{
	curl_global_cleanup();
}

bool SftpClient::setPasswordFilePath(const std::string &password_file_path)
{
	ifstream file_stream(password_file_path);
	if(!file_stream.is_open())
	{
		cout << "Error: Cannot open password_file_path" << endl;
		return false;
	}
	
	file_stream >> _password;
	return true;
}

bool SftpClient::setKnownHostsFilePath(const std::string &known_hosts_file_path)
{
	_known_hosts_path = known_hosts_file_path;
	if(ifstream{known_hosts_file_path}.is_open())
		return true;

	cout << "Error: Cannot open known_hosts_file_path" << endl;
	return false;
}

bool SftpClient::setPublicKeyFilePath(const std::string &public_key_file_path)
{
	_public_key_path = public_key_file_path;
	if(ifstream{public_key_file_path}.is_open())
		return true;

	cout << "Error: Cannot open public_key_file_path" << endl;
	return false;
}

bool SftpClient::setPrivateKeyFilePath(const std::string &private_key_file_path)
{
	_private_key_path = private_key_file_path;
	if(ifstream{private_key_file_path}.is_open())
		return true;

	cout << "Error: Cannot open private_key_file_path" << endl;
	return false;
}

// server_file_path 	should be "/directory/../file_name"
// save_file_path 		can be left empty, then it will be the filename from the server_file_path
bool SftpClient::getFile(const std::string &server_file_path, const std::string &save_file_path_const)
{
	_curl = curl_easy_init();
	if(_curl == nullptr)
	{
		cout << "Error: Cannot init curl" << endl;
		return false;
	}
	
	string save_file_path = save_file_path_const;
	if(save_file_path.empty())
	{
		// get file name from last string of server_file_path
		auto last_string_pos = server_file_path.rfind('/');
		if(last_string_pos == string::npos)
		{
			cout << "Error: Empty server file name" << endl;
			curl_easy_cleanup(_curl);
			return false;
		}
		save_file_path = server_file_path.substr(last_string_pos + 1);
	}
	
	if(ifstream{save_file_path}.is_open())
	{
		cout << "Error: File already exist: " << save_file_path << endl;
		curl_easy_cleanup(_curl);
		return false;
	}
	
	string url = "sftp://" + _username + "@" + _ip + server_file_path;
	SftpFile sftp_file{save_file_path, nullptr};
	
	curl_easy_setopt(_curl, CURLOPT_URL,					url.c_str());
	curl_easy_setopt(_curl, CURLOPT_USERNAME,				_username.c_str());
	curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION,			writeCallback);
	curl_easy_setopt(_curl, CURLOPT_WRITEDATA,				&sftp_file);
	curl_easy_setopt(_curl, CURLOPT_SSH_AUTH_TYPES,			CURLSSH_AUTH_PUBLICKEY | CURLSSH_AUTH_PASSWORD);
	curl_easy_setopt(_curl, CURLOPT_SSH_KNOWNHOSTS,			_known_hosts_path.c_str());
	curl_easy_setopt(_curl, CURLOPT_SSH_PUBLIC_KEYFILE,		_public_key_path.c_str());
	curl_easy_setopt(_curl, CURLOPT_SSH_PRIVATE_KEYFILE,	_private_key_path.c_str());
	curl_easy_setopt(_curl, CURLOPT_KEYPASSWD,				_password.c_str());
	
	// uncomment for full debug
	// curl_easy_setopt(_curl, CURLOPT_VERBOSE,				1L);

	CURLcode return_value = curl_easy_perform(_curl);
	if(return_value != CURLE_OK)
	{
		cout << "Error: Perform curl fail: " << return_value << endl;
		curl_easy_cleanup(_curl);
		if(sftp_file.stream)
			fclose(sftp_file.stream);
		return false;
	}
	
	curl_easy_cleanup(_curl);
	if(sftp_file.stream)
		fclose(sftp_file.stream);
	
	return true;
}