#include "OcrLite.h"
#include "main.h"

void printHelp(FILE *out, const char *argv0) {
    fprintf(out, " ------- Usage -------\n");
    fprintf(out, "%s %s", argv0, usageMsg);
    fprintf(out, " ------- Required Parameters -------\n");
    fprintf(out, "%s", requiredMsg);
    fprintf(out, " ------- Optional Parameters -------\n");
    fprintf(out, "%s", optionalMsg);
    fprintf(out, " ------- Other Parameters -------\n");
    fprintf(out, "%s", otherMsg);
    fprintf(out, " ------- Examples -------\n");
    fprintf(out, example1Msg, argv0);
    fprintf(out, example2Msg, argv0);
}

int main(int argc, char **argv) {
    if (argc <= 1) {
        printHelp(stderr, argv[0]);
        return -1;
    }

    std::string modelsDir, argImgPath, imgPath, imgName;
    modelsDir = "../models";
    argImgPath = "../test_imgs/1.jpg";
    int numThread = 4;
    int padding = 50;
    int imgResize = 0;
    float boxScoreThresh = 0.6f;
    float boxThresh = 0.3f;
    float minArea = 3.f;
    float unClipRatio = 2.0f;
    bool doAngle = true;

    int opt;
    int optionIndex = 0;
    while ((opt = getopt_long(argc, argv, "i:d:t:p:s:b:o:m:u:nV?", long_options, &optionIndex)) != -1) {
        //printf("option(-%c)=%s\n", opt, optarg);
        switch (opt) {
            case 'd':
                modelsDir = optarg;
                printf("modelsPath=%s\n", modelsDir.c_str());
                break;
            case 'i':
                argImgPath = std::string(optarg);
                imgPath = argImgPath.substr(0, argImgPath.find_last_of('/') + 1);
                imgName = argImgPath.substr(argImgPath.find_last_of('/') + 1);
                printf("imgPath=%s, imgName=%s\n", imgPath.c_str(), imgName.c_str());
                break;
            case 't':
                numThread = (int) strtol(optarg, NULL, 10);
                printf("numThread=%d\n", numThread);
                break;
            case 'p':
                padding = (int) strtol(optarg, NULL, 10);
                printf("padding=%d\n", padding);
                break;
            case 's':
                imgResize = (int) strtol(optarg, NULL, 10);
                printf("imgResize=%d\n", imgResize);
                break;
            case 'b':
                boxScoreThresh = strtof(optarg, NULL);
                printf("boxScoreThresh=%f\n", boxScoreThresh);
                break;
            case 'o':
                boxThresh = strtof(optarg, NULL);
                printf("boxThresh=%f\n", boxThresh);
                break;
            case 'm':
                minArea = strtof(optarg, NULL);
                printf("minArea=%f\n", minArea);
                break;
            case 'u':
                unClipRatio = strtof(optarg, NULL);
                printf("unClipRatio=%f\n", unClipRatio);
                break;
            case 'n':
                doAngle = false;
                printf("noAngle=%d\n", !doAngle);
                break;
            case '?':
                printHelp(stdout, argv[0]);
                break;
            default:
                printf("other option %c :%s\n", opt, optarg);
        }
    }

    OcrLite ocrLite(numThread);
    ocrLite.initLogger(imgPath.c_str(), imgName.c_str(),
                       true,//isOutputConsole
                       false,//isOutputPartImg
                       false,//isOutputAngleImg
                       false,//isOutputDebugImg
                       true,//isOutputResultTxt
                       true);//isOutputResultImg
    bool ret = ocrLite.initModels(modelsDir.c_str());
    if (!ret) return -1;

    OcrResult result = ocrLite.detect(imgPath.c_str(), imgName.c_str(),
                                      padding, imgResize,
                                      boxScoreThresh, boxThresh, minArea,
                                      unClipRatio, doAngle);
    ocrLite.Logger("%s\n", result.strRes.c_str());

    return 0;
}
