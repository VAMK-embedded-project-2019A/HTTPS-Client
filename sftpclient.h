#ifndef SFTPCLIENT_H
#define SFTPCLIENT_H

#include <string>
#include <curl/curl.h>
#undef DISABLE_SSH_AGENT

size_t writeCallback(void *buffer, size_t size, size_t nmemb, void *stream);

class SftpClient
{
public:
	SftpClient() = delete;
	SftpClient(const SftpClient &) = delete;
	SftpClient(const std::string &ip, const std::string &username);
	~SftpClient();
	
	void setServerFilePath(const std::string &server_file_path);
	bool setPasswordFilePath(const std::string &password_file_path);
	bool setKnownHostsFilePath(const std::string &known_hosts_file_path);
	bool setPublicKeyFilePath(const std::string &public_key_file_path);
	bool setPrivateKeyFilePath(const std::string &private_key_file_path);
	
	// server_file_path 	should be "/directory/../file_name"
	// save_file_path 		can be left empty, then it will be the filename from the server_file_path
	bool getFile(const std::string &server_file_path, const std::string &save_file_path_const = std::string{});
	
private:
	CURL *_curl{nullptr};

	std::string _ip;
	std::string _username;
	std::string _password;
	std::string _known_hosts_path;
	std::string _public_key_path;
	std::string _private_key_path;
};

#endif // SFTPCLIENT_H