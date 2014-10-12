#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include "configure.h"

std::pair< std::map<std::string,std::string>,std::vector<std::string> > getConfiguration()
{
   Config cfg;

   try
   {
      cfg.readFile("config/config.json");
   }
   catch(const FileIOException &fioex)
   {
        std::cerr << "I/O error while reading config/config.json" << std::endl;
        exit(EXIT_FAILURE);
   }
   catch(const ParseException &pex)
   {
        std::cerr << "Configuration file parse error at " << pex.getFile() << ":" << pex.getLine()
              << " - " << pex.getError() << std::endl;
        exit(EXIT_FAILURE);
   }

   try
   {
        const Setting& root = cfg.getRoot();
        const Setting &backend = root[ACTIVE_TABLES][BACKEND_OPTIONS];
        const Setting &model_list = root[ACTIVE_TABLES][MODEL_LIST];
        std::map<std::string,std::string> _config;
        std::vector <std::string> _model_list;

        std::string str_temp_model_list;
        
        std::istream_iterator<std::string> begin(ss);
        std::istream_iterator<std::string> end;
        std::vector<std::string> vstrings(begin, end);
        std::copy(vstrings.begin(), vstrings.end(), std::ostream_iterator<std::string>(std::cout, "\n"));

        if (!(backend.lookupValue(USERNAME, _config["db_username"]) &&
              backend.lookupValue(PASSWORD, _config["db_password"]) &&
              backend.lookupValue(HOSTNAME, _config["db_hostname"]) &&
              backend.lookupValue(DBNAME, _config["db_dbname"]) &&
              backend.lookupValue(PORT, _config["db_port"])))
        {
			std::cerr<<"Error while getting backend config from config/config.json"<<std::endl;
        }
        if (!(model_list.lookupValue(MODEL_LIST, str_temp_model_list)))
        {
            std::cerr<<"Error while getting model list config from config/config.json"<<std::endl;
        }
        if (str_temp_model_list.empty())
        {
            std::cerr<<"Retrived empty list of models from config/config.json"<<std::endl;
        }

        std::stringstream ss(str_temp_model_list);
        std::istream_iterator<std::string> begin(ss);
        std::istream_iterator<std::string> end;
        std::vector<std::string> _model_list(begin, end);
        std::copy(_model_list.begin(), _model_list.end(), std::ostream_iterator<std::string>(std::cout, ","));

   }
   catch(const SettingNotFoundException &nfex)
   {
        std::cerr<<"missing configuration at config/config.json"<<std::endl;
   }
   return std::make_pair(_config,_model_list);
}
