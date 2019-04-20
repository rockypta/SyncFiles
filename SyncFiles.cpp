#include<iostream>
#include<fstream>
#include<string>
#include<sys/types.h>
#include<dirent.h>
#include<vector>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

bool IsDirectory(std::string sName)
{
    DIR* directory = opendir(sName.c_str());

    if(directory != NULL)
    {
     closedir(directory);
     return true;
    }

    return false;
}

void GetAllFiles(std::string sPath, std::vector<std::string>& vListOfPath)
{
    vListOfPath.clear();

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (sPath.c_str())) != NULL) 
	{
		while ((ent = readdir (dir)) != NULL) 
		{
			if (ent->d_name[0] != '.') 
				vListOfPath.push_back(ent->d_name);
		}
		closedir (dir);
    }
}

bool SyncFiles(std::string sOrigPath, std::string sCopyPath)
{
    std::vector<std::string> vOrginRecPaths;
    GetAllFiles(sOrigPath, vOrginRecPaths);

    std::vector<std::string> vCopyRecPaths;
    GetAllFiles(sCopyPath, vCopyRecPaths);

    for(int i = 0; i < (int)vOrginRecPaths.size(); i++)
	{
		bool bFound = false;
		int iFoundLocation = -1;
		for(int j = 0; j < (int)vCopyRecPaths.size(); j++)
		{
			if(vOrginRecPaths.at(i).compare(vCopyRecPaths.at(j)) == 0)
			{
				bFound = true;
				iFoundLocation = j;
				break;
			}
		}

		if(bFound && IsDirectory(sOrigPath + "/" + vOrginRecPaths.at(i)))
			SyncFiles (sOrigPath + "/" + vOrginRecPaths.at(i), sCopyPath+ "/" + vCopyRecPaths.at(iFoundLocation));
		else if(bFound)
			std::cout << "Found :" << i << std::endl; 
		else
		{
			try
			{	
				std::cout << "............................................ " << std::endl;
				std::cout << "............................................ " << std::endl;
				std::cout << "............................................ " << std::endl;
				std::cout << "............................................ " << std::endl;
				std::cout << "Copying... " <<vOrginRecPaths.at(i) << std::endl;
				if(IsDirectory(sOrigPath + "/" + vOrginRecPaths.at(i)))
					fs::copy(sOrigPath + "/" + vOrginRecPaths.at(i), sCopyPath + "/" + vOrginRecPaths.at(i), fs::copy_options::recursive);
				else	
					fs::copy(sOrigPath + "/" + vOrginRecPaths.at(i), sCopyPath);
					
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
			
		}
		
	}
    return true;
}



int main(int iArgc, char* aArgv[]) 
{	
	if (iArgc < 2) 
	{
		std::cout << "You need to provide a valid directory" << std::endl;
	}

    std::string vSoucePath;
    if (iArgc >= 2) 
    	vSoucePath = std::string(aArgv[1]);
    else
    {
        std::cin >> vSoucePath;
    }
    
	if (!IsDirectory(vSoucePath.c_str()))
	{
		std::cout << "Not a valid directory";
		return 2;
	}

    std::string vCopyPath;
    if (iArgc >= 3) 
    	vCopyPath = std::string(aArgv[2]);
    else
    {
        std::cin >> vCopyPath;
    }
    
    if (!IsDirectory(vCopyPath.c_str())) 
	{
		std::cout << "Not a valid copy direcotry";
		return 2;
	}


	return 	(SyncFiles(vSoucePath.c_str(), vCopyPath.c_str())? 0 : 2);
}