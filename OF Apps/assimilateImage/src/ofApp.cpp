#include "ofApp.h"

ofApp::ofApp() : dataset(), kdtree(3, dataset, nanoflann::KDTreeSingleIndexAdaptorParams(10))
{
    
}

ofApp::ofApp(ImageDataSet & a_dataset) :  kdtree(3, a_dataset, nanoflann::KDTreeSingleIndexAdaptorParams(10) )
{
    dataset = a_dataset;
}


//--------------------------------------------------------------
void ofApp::setup(){
    
    dir.open( DIR_PATH );
    result_dir.open( RESULT_DIR );
    template_dir.open( TEMPLATE_DIR );
    queue_dir.open( QUEUE_DIR );
    
    
    if (!dir.exists() || !result_dir.exists() || !template_dir.exists() || !queue_dir.exists() ) ofExit();
    
    
    templateFile = getLatestFile(template_dir, "png");
    
    if (templateFile == ""  || !loadTemplate(TEMPLATE_DIR + templateFile)) ofExit();
    
    
    latestResult = getLatestFile(result_dir, "png");
    
    if (latestResult != "" ) {
        //deletePrevious(result_dir, latestResult);
        resultImage.loadImage(RESULT_DIR + latestResult);
        resultImage.resize(RESULT_WIDTH, RESULT_HEIGHT);
    } else {
        resultImage.allocate(templateImagePixels.getWidth(), templateImagePixels.getHeight(), OF_IMAGE_COLOR);
        memset(resultImage.getPixels(), 0, resultImage.getPixelsRef().size());
    }
    
    
    
#if FRAME_RATE > 0
    ofSetFrameRate(FRAME_RATE);
#endif
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if ( ! (++count %= UPDATE_TEMPLATE) ) updateTemplate();
    
    unsigned char* result = resultImage.getPixels();
    
    const int size = resultImage.getPixelsRef().size();
    
    //count = ++count % 200;
    //if (!count) fade(result, resultImage.getPixelsRef().size());
    //resultImage.update();
    
    there_was_buffer = there_is_buffer;
    
    to_feed = MAX_TO_FEED;
    while (to_feed && ( (remaining > 0) || (there_is_buffer = remaining = getBuffer()) ) )
    {
        if (to_feed == MAX_TO_FEED && !is_random ) fade(result, size);
        feed(result);
    }
    
    if (there_is_buffer || there_was_buffer)
    {
        resultImage.update();
        ostringstream stream;
        
        stream << ofGetUnixTime();
        stream << ofGetSystemTimeMicros();
        stream << ".png";
        string filename = stream.str();
        string path = RESULT_DIR + filename;
        resultImage.saveImage(path);
        //cout << "Image Saved: " << path << "\n";
        
#if SAVE_RESULT == 0
        if ( previousResult != "" ) ofFile::removeFile( RESULT_DIR + previousResult );
        
        result_dir.open( RESULT_DIR );
        if (!result_dir.exists()) ofExit();
        
        previousResult = latestResult;
        latestResult = filename;
#endif
    }
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //resultImage.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::updateTemplate() {
    
    string newTemplate = getFirstFile(queue_dir, "png");
    
    if (newTemplate == "" || newTemplate == templateFile) return;
    
    rename( (QUEUE_DIR + newTemplate).c_str(), (TEMPLATE_DIR + newTemplate).c_str() );
    
    if (!loadTemplate(TEMPLATE_DIR + newTemplate)) return;
    
    
    if (templateFile != "" ) ofFile::removeFile( TEMPLATE_DIR + templateFile );
    
    template_dir.open( TEMPLATE_DIR );
    if (!template_dir.exists()) ofExit();
    
    
    templateFile = newTemplate;
}

//--------------------------------------------------------------
bool ofApp::loadTemplate(const string path) {
    
    if (!ofLoadImage(templateImagePixels, path)) return 0;
    
    templateImagePixels.resize(RESULT_WIDTH, RESULT_HEIGHT);
    
    templateImagePixels.setImageType(OF_IMAGE_COLOR);
    
    dataset.set(templateImagePixels.getPixels(), templateImagePixels.size());
    
    kdtree.buildIndex();
    
    return 1;
}


//--------------------------------------------------------------
void ofApp::fade(unsigned char* data, int i) {
    while(i--) {
        if (data[i]){
            data[i] *= FADE_FACT;
        }
    }
}

//--------------------------------------------------------------
void ofApp::feed(unsigned char* result) {
    
    int i = remaining - 3;
    size_t pix_pos;
    size_t pix_index;
    unsigned char pix_value[3];
    
    if (is_random) {
        
        pix_index = rand_pos;
        pix_value[0] = rand_value[0];
        pix_value[1] = rand_value[1];
        pix_value[2] = rand_value[2];
        
        int value = rand_pos + (std::rand() % RAND_POS_RANGE) - RAND_POS_RANGE/2;
        value += ((std::rand() % RAND_POS_RANGE) - RAND_POS_RANGE/2) * templateImagePixels.getWidth();
        
        rand_pos = (value >= dataset.pix_num) ? dataset.pix_num - 1 : (value < 0) ? 0 : value;
        
        
        value = rand_value[0] + (std::rand() % RAND_VALUE_RANGE) - RAND_VALUE_RANGE/2;
        rand_value[0] = ( value > 255 ) ? 255 : (value < 0 ) ? 0 : value;
        
        value = rand_value[1] + (std::rand() % RAND_VALUE_RANGE) - RAND_VALUE_RANGE/2;
        rand_value[1] = ( value > 255 ) ? 255 : (value < 0 ) ? 0 : value;
        
        value = rand_value[2] + (std::rand() % RAND_VALUE_RANGE) - RAND_VALUE_RANGE/2;
        rand_value[2] = ( value > 255 ) ? 255 : (value < 0 ) ? 0 : value;
        
        if (++rand_count >= RAND_RESET) {
            
            value = rand_pos_center + (std::rand() % RAND_POS_RANGE) - RAND_POS_RANGE/2;
            value += ((std::rand() % RAND_POS_RANGE) - RAND_POS_RANGE/2) * templateImagePixels.getWidth();
            
            rand_pos_center = (value >= dataset.pix_num) ? dataset.pix_num - 1 : (value < 0) ? 0 : value;
            
            rand_pos = rand_pos_center;
            
            value = rand_value_center[0] + (std::rand() % RAND_VALUE_RANGE) - RAND_VALUE_RANGE/2;
            rand_value_center[0] = ( value > 255 ) ? 255 : (value < 0 ) ? 0 : value;
            
            value = rand_value_center[1] + (std::rand() % RAND_VALUE_RANGE) - RAND_VALUE_RANGE/2;
            rand_value_center[1] = ( value > 255 ) ? 255 : (value < 0 ) ? 0 : value;
            
            value = rand_value_center[2] + (std::rand() % RAND_VALUE_RANGE) - RAND_VALUE_RANGE/2;
            rand_value_center[2] = ( value > 255 ) ? 255 : (value < 0 ) ? 0 : value;
            
            rand_value[0] = rand_value_center[0];
            rand_value[1] = rand_value_center[1];
            rand_value[2] = rand_value_center[2];
            
            rand_count = 0;
        };
        
    }
    else {
        // do a knn search
        const size_t num_results = 1;
        int shortest_sqrdist;
        nanoflann::KNNResultSet<int> resultSet(1);
        resultSet.init(&pix_index, &shortest_sqrdist);
        kdtree.findNeighbors(resultSet, &data_ptr[i], nanoflann::SearchParams(10));
        
        dataset.remove(pix_index);
        
        pix_value[0] = data_ptr[i];
        pix_value[1] = data_ptr[i+1];
        pix_value[2] = data_ptr[i+2];
    }
    
    pix_pos = pix_index * 3;
    
    result[pix_pos] = pix_value[0];
    result[pix_pos+1] = pix_value[1];
    result[pix_pos+2] = pix_value[2];
    
    remaining = i;
    to_feed--;
    
    //cout << to_feed << " " << remaining << "\n";
}

//--------------------------------------------------------------
int ofApp::getBuffer(){
    
    size_t size;
    
    string fileToOpen = getLatestFile(dir, "ebb");
    
    if (fileToOpen != "") {
        
        cout << "file to open: " << fileToOpen << "\n";
        
        string filePath = DIR_PATH + fileToOpen;
        
        if ( fileToOpen.substr(fileToOpen.length() - 5) == "R.ebb" ) {
            
            size = RAND_SIZE;
            is_random = true;
            rand_pos_center = rand_pos = std::rand() % dataset.pix_num;
            size_t i = rand_pos * 3;
            rand_value_center[0] = rand_value[0] = dataset.imageData[i];
            rand_value_center[1] = rand_value[1] = dataset.imageData[i+1];
            rand_value_center[2] = rand_value[2] = dataset.imageData[i+2];
            
        }
        else {
            
            imageData.clear();
            
            imageData = ofBufferFromFile(filePath);
            
            data_ptr = (unsigned char*)imageData.getBinaryBuffer();
            
            cout << "buffer size: " << imageData.size() << "\n";
            
            size = (imageData.size() / 3) * 3;
            
            is_random = false;
            
        }
        
        ofFile::removeFile(filePath);
        
        dir.open(DIR_PATH);
        if (!dir.exists()) ofExit();
        
        return size;
        
    }
    
    return 0;
}

//--------------------------------------------------------------
string ofApp::getLatestFile( ofDirectory& directory, const string ext){
    
    if (!directory.exists())
    {
        return "";
    }
    
    directory.open( directory.path() );
    directory.allowExt(ext);
    directory.listDir();
    
    unsigned int i = directory.size();
    
    string latestFile = "";
    
    bool got = false;
    while (i--) {
        string currentFile = directory.getName(i);
        if ( !got || (currentFile.compare(latestFile) > 0) ){
            latestFile = currentFile;
            got = true;
        }
    }
    
    return latestFile;
    
}

//--------------------------------------------------------------
string ofApp::getFirstFile( ofDirectory& directory, const string ext){
    
    if (!directory.exists())
    {
        return "";
    }
    
    directory.open( directory.path() );
    directory.allowExt(ext);
    directory.listDir();
    
    unsigned int i = directory.size();
    
    string firstFile = "";
    
    bool got = false;
    while (i--) {
        string currentFile = directory.getName(i);
        if ( !got || (currentFile.compare(firstFile) < 0) ){
            firstFile = currentFile;
            got = true;
        }
    }
    
    return firstFile;
    
}

//--------------------------------------------------------------
bool ofApp::deletePrevious( ofDirectory& directory, string filename) {
    
    if (!directory.exists())
    {
        return 0;
    }
    
    directory.open( directory.path() );
    directory.allowExt( ofFilePath::getFileExt(filename) );
    directory.listDir();
    
    unsigned int i = directory.size();
    
    string* toDelete = new string [i];
    
    unsigned int n = 0;
    
    while (i--) {
        string currentFile = directory.getName(i);
        if ( currentFile.compare(filename) < 0 ){
            toDelete[n] = currentFile;
            n++;
        }
    }
    
    while (n--) {
        ofFile::removeFile( directory.path() + "/" + toDelete[n] );
    }
    
    delete [] toDelete;
    
    directory.open( directory.path() );
    
    return 1;
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
