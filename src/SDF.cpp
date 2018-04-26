#include <iostream>
#include <sdf/sdf.hh>

int main(int argc, char **argv)
{

    // Create and initialize the data structure that will hold the parsed SDF data
    sdf::SDFPtr sdfParsed(new sdf::SDF());
    sdf::init(sdfParsed);

    // Read an SDF file, and store the result in sdfParsed.
    if (sdf::readFile("/home/leon/Instrument_Pose_Estimation/Test_C/models/roboy/model.sdf", sdfParsed))
    {
        // Get a pointer to the model element
        sdf::ElementPtr model = sdfParsed->Root()->GetElement("model");

        // Get the "name" attribute from the model
        std::string modelName = model->Get<std::string>("name");

        std::cout << "Model name = " << modelName << "\n";

        // Get the child "link" element, and its name
        auto link = model->GetElement("link");
        auto linkName = link->Get<std::string>("name");

        std::cout << "Link name = " << linkName << "\n";
        return 0;
    }

    return -1;
}