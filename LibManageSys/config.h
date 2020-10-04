#pragma once
#include <json/json.h>
#include <fstream>
#include <string>

using std::string;

//设置储存在"info\config.json"文件中，请直接修改配置json文件完成修改操作

static struct Config
{
	int maxBorrowTime;//单次借阅最长可借时间（不包含续借）
	int renewTime;//单次续借延长时间
	int delayFineRatio;//滞纳金罚款倍率（滞纳金=罚款倍率*延期本数*延期时间，单位分）
	int lostFineRatio;//书籍丢失罚款倍率（丢失罚款=书籍标价*罚款倍率，单位分）
} config;

static void loadConfig()
{
	string filename = "info\\config.json";
	std::ifstream in(filename);
	Json::Value root;
	Json::CharReaderBuilder rbuilder;
	rbuilder["collectComments"] = false;
	string errs;

	if (Json::parseFromStream(rbuilder, in, &root, &errs))
	{
		config.maxBorrowTime = root["maxBorrowTime"].asInt();
		config.renewTime = root["renewTime"].asInt();
		config.delayFineRatio = root["delayFineRatio"].asInt();
		config.lostFineRatio = root["lostFineRatio"].asInt();
	}
	in.close();
}

static void saveConfig(Config config)
{
	Json::Value root;
	root["maxBorrowTime"] = Json::Value(config.maxBorrowTime);
	root["renewTime"] = Json::Value(config.renewTime);
	root["delayFineRatio"] = Json::Value(config.delayFineRatio);
	root["lostFineRatio"] = Json::Value(config.lostFineRatio);

	std::ofstream os;
	std::string filename = "info\\config.json";
	os.open(filename, std::ios::out | std::ios::app);
	os << root;
	os.close();
}
