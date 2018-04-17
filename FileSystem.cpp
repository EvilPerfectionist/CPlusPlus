#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: tut1 path \n" << argc << std::endl;
        return 1;
    }
    path p(argv[1]);  // avoid repeated path construction below

    try
    {
        if (exists(p))
        {
            if (is_regular_file(p))
                cout << p << " size is " << file_size(p) << '\n';

            else if (is_directory(p))
            {
                cout << p << " is a directory containing:\n";

                std::vector<path> v;


                for (directory_iterator x = directory_iterator(p); x != directory_iterator(); x++)
                    v.push_back(x->path());
                sort(v.begin(), v.end());

                for (unsigned i=0; i < v.size(); ++i)
                    cout << "    " << v[i].filename() << '\n';
            }
            else
                cout << p << " exists, but is not a regular file or directory\n";
        }
        else
            cout << p << " does not exist\n";
    }

    catch (const filesystem_error& ex)
    {
        cout << ex.what() << '\n';
    }

    return 0;
}
