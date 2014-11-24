#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    dir.open( DIR_PATH );
    result_dir.open( RESULT_DIR );
    template_dir.open( TEMPLATE_DIR );
    queue_dir.open( QUEUE_DIR );
    
    if (!dir.exists() || !result_dir.exists() || !template_dir.exists() || !queue_dir.exists()) ofExit();
    
    latestResult = getLatestFile(result_dir, "txt");
    
    //if (latestResult != "") deletePrevious(result_dir, latestResult);
    
    
    templateFile = getLatestFile(template_dir, "txt");
    
    if (templateFile == "" ) ofExit();
    
    loadTemplate( TEMPLATE_DIR + templateFile );
    
    deletePrevious(template_dir, templateFile);
    
    std::srand((unsigned int)ofGetSystemTimeMicros());
    
    
#if ( FRAME_RATE > 0 )
    ofSetFrameRate(FRAME_RATE);
#endif
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //ofSleepMillis(10);
    
    if ( ! (++count %= UPDATE_TEMPLATE) ) updateTemplate();
    
    there_was_buffer = there_is_buffer;
    
    to_feed = MAX_TO_FEED;
    while (to_feed)
    {
        if ( (remaining > 0) || (there_is_buffer = remaining = getBuffer()) )
        {
            feed();
        }
        else
        {
            break;
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (there_was_buffer || there_is_buffer) {
        size_t size = numBytes(resultText);
        char* output = new char [size];
        wcharToChar(output, resultText);
        ostringstream stream;
        stream << ofGetUnixTime();
        stream << ofGetSystemTimeMicros();
        stream << ".txt";
        string filename = stream.str();
        ofBufferToFile( RESULT_DIR + filename, *new ofBuffer(output, size));
        //cout << string( 100, '\n' );
        cout << output << "\n";
        //ofDrawBitmapString(output, 100, 100);
        
#if SAVE_RESULT == 0
        if (previousResult != "" ) ofFile::removeFile( RESULT_DIR + previousResult );
        
        result_dir.open( RESULT_DIR );
        if (!result_dir.exists()) ofExit();
        
        previousResult = latestResult;
        latestResult = filename;
#endif
    }
}

//--------------------------------------------------------------
void ofApp::updateTemplate() {
    
    string newTemplate = getFirstFile(queue_dir, "txt");
    
    if (newTemplate == "" || newTemplate == templateFile) return;
    
    rename( (QUEUE_DIR + newTemplate).c_str(), (TEMPLATE_DIR + newTemplate).c_str() );
    
    loadTemplate( TEMPLATE_DIR + newTemplate );
    
    
    if (templateFile != "" ) ofFile::removeFile( TEMPLATE_DIR + templateFile );
    
    template_dir.open( TEMPLATE_DIR );
    if (!template_dir.exists()) ofExit();
    
    
    templateFile = newTemplate;
    
}

//--------------------------------------------------------------
void ofApp::loadTemplate(const string path) {
    
    string s = ofBufferFromFile(path).getText();
    
    cout << "str size: " << s.size() << "\n";
    
    template_length = charToWchar(templateText, s.c_str());
    
    resultText[template_length] = 0;
    
}

//--------------------------------------------------------------
int ofApp::getBuffer() {
    
    string fileToOpen = getLatestFile(dir, "ebb");
    
    if (fileToOpen != "") {
        
        size_t numChars;
        
        cout << "file to open: " << fileToOpen << "\n";
        
        string filePath = DIR_PATH + fileToOpen;
        
        
        if ( fileToOpen.substr(fileToOpen.length() - 5) == "R.ebb" ) {
            
            numChars = RAND_SIZE;
            is_random = true;
            rand_pos = std::rand() % template_length;
            rand_value = templateText[rand_pos];
            
        }
        else {
        
            textData.clear();
            delete [] wide_data;
            
            textData = ofBufferFromFile(filePath);
            
            size_t size = textData.size();
            
            cout << "buffer size: " << size << "\n";
            
            wide_data = new unsigned long [size];
            
            numChars = charToWchar(wide_data, textData.getBinaryBuffer());
            
            is_random = false;
        }
        
        
        ofFile::removeFile(filePath);
        
        dir.open(DIR_PATH);
        if (!dir.exists()) ofExit();
        
        return numChars;
        
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
void ofApp::feed(){
    
    remaining--;
    
    
    unsigned long current;
    
    size_t dest = 0;
    
    if (is_random) {
        
        dest = rand_pos;
        current = rand_value;
        
        rand_pos += (std::rand() % RAND_POS_RANGE) - RAND_POS_RANGE/2;
        rand_pos %= template_length;
        rand_value += (std::rand() % RAND_VALUE_RANGE) - RAND_VALUE_RANGE/2;
    }
    else {
        
        current = wide_data[remaining];
        
        long long closestDistance =  compareDistances(templateText[0], resultText[0], current);
        long long currentDistance;
        
        for (size_t i = 1; i < template_length; i++){
            currentDistance = compareDistances(templateText[i], resultText[i], current);
            if ( currentDistance > closestDistance ) {
                closestDistance = currentDistance;
                dest = i;
            }
        }
    }
    
    resultText[dest] = current;
    
    to_feed--;
}

//---------------------------------------------------------------
inline long long ofApp::compareDistances(unsigned long origin, unsigned long dest1, unsigned long dest2) {
    return abs((long long)origin - (long long)dest1) - abs((long long)origin - (long long)dest2);
}

//--------------------------------------------------------------
size_t ofApp::charToWchar(unsigned long* dest, const char* source) {
    size_t i = 0;
    size_t wi = 0;
    while ( source[i] != 0 ) {
        unsigned long current = (unsigned char)source[i];
        i++;
        if ( (char)current < 0 ) {
            while ( (source[i] < (char)0300) ) {
                current = (current << 8) | (source[i] & 0xff);
                i++;
            }
        }
        dest[wi] = current;
        wi++;
    }
    dest[wi] = 0;
    
    return wi;
}

//--------------------------------------------------------------
size_t ofApp::wcharToChar(char* dest, const unsigned long* source){
    
    size_t i = 0;
    size_t di = 0;
    while ( source[i] != 0 ) {
        if (source[i] & 0x80000000) dest[di++] = (source[i] >> 24) & 0xff;
        if (source[i] & 0x00800000) dest[di++] = (source[i] >> 16) & 0xff;
        if (source[i] & 0x00008000) dest[di++] = (source[i] >> 8) & 0xff;
        dest[di++] = source[i] & 0xff;
        i++;
    }
    dest[di] = 0;
    
    return di;
}

//--------------------------------------------------------------
size_t ofApp::numBytes(unsigned long* wide){
    
    size_t n = 1;
    
    for (size_t i = 0; wide[i] != 0; i++) {
        n++;
        if (wide[i] & 0x00008000) n++;
        if (wide[i] & 0x00800000) n++;
        if (wide[i] & 0x80000000) n++;
    }
    
    return n;
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
