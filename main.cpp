#include <iostream>
#include <sstream>
#include "csvstream.hpp"
#include <string>
#include <map>
#include <set> 
#include <limits> 
#include <cmath>

using namespace std;

class Classifier {

private:

    //The total number of posts in the entire training set.
    int totalPosts; 
    set<string> uniqueLabels;
    //The number of unique words in the entire training set. (The vocabulary size.)
    int vocabularySize;
    //For each word w, the number of posts in the entire training set that contain it 
    map <string, int> postswWord;
    // For each label, C , the number of posts with that label.
    map <string, int> postswLabel ;
    // For each label , C , and word w, the number of posts with label C that contain w 
    map <pair<string,string>, int> postsWLpairs; 

    // 
    set<string> allLabels; 
    set<string> allWords; 


    //helper functions for training 

    // EFFECTS: Return a set of unique whitespace delimited words.
    set<string> unique_words(const string &str) {
        istringstream source(str);
        set<string> words;
        string word;
        while (source >> word) {
             words.insert(word);
        }
        return words;
    }

    set<string> uniqueWords(const string &filename){
        // Open file
        csvstream csvin(filename);

        //create set to store unique Words
        set<string> uniqueWords;

        //create map to store row data
        map<string, string> row;

        while (csvin >> row) {
            set<string> words = unique_words(row["content"]);
            uniqueWords.insert(words.begin(), words.end());
             
        }

        return uniqueWords;

    }


    //log-proior probability 
    double logPriorProb(const string &label){
        double labelCount = static_cast<double>(postswLabel[label]);
        return log(labelCount / (double)totalPosts);
        
    }

    //loglikelihood 
     double logLikelihood(const string &label, const string &word){

        int tpostswLabel = postswLabel[label]; 

        //bool check3 = postsWLpairs.find(make_pair(label, word)) == postsWLpairs.end();
       // bool check4 = postswWord.find(word) != postswWord.end(); 

        
        //if w does not occur anywhere
         if (postswWord.find(word) == postswWord.end()){  //|| postswWord[word]== 0
                        return log(1.0/(double)totalPosts); 
                    }

       
         //if w occurs but not in that label 
         else if((postsWLpairs.find(make_pair(label, word)) 
         == postsWLpairs.end() && postswWord.find(word) != postswWord.end())){ // ){

        
                        return log(((double)postswWord[word])/(double)totalPosts); 
                    }

        // regular formula 
        else{ return log((double)postsWLpairs
        [make_pair(label, word)]/(double)tpostswLabel); }

    }




     
public: 
    
    // training function 

    void training(const string &filename, bool debug){
       
        csvstream csvin(filename);
        map<string, string> row;

        if (debug == true){
            cout << "training data:" << endl;
        }

        totalPosts = 0; 
        vocabularySize = (uniqueWords(filename)).size();

       
            
        while (csvin >> row) {

            string label = row["tag"]; 
            string content = row["content"]; 

            if(debug == true){
                cout << "  label = " << label << ", content = " << content << endl;
            }

            allLabels.insert(label); 
            
            ++ totalPosts;
            set<string> words = unique_words(row["content"]);


                for (const auto &word : words) {
                    if (words.find(word) != words.end()){
                        ++ postswWord[word];
                     }
                }

                if (row["tag"] == label){
                        ++ postswLabel[row["tag"]];

                        for (const auto &word : words) {
                             if (words.find(word) != words.end()){
                              ++ postsWLpairs[make_pair(label,word)];
                             }

                         }
                }
                       
            }



        
        

        cout << "trained on " << totalPosts << " examples" << endl;

        allWords = uniqueWords(filename);


        if (debug == true){
            cout << "vocabulary size = " << vocabularySize << endl<<endl; 
            cout << "classes:" << endl; 
            for (const auto &label : allLabels) {
                cout << "  "<< label << ", " << postswLabel[label] 
                << " examples, log-prior = " << logPriorProb(label) 
                << endl;
            }
            
            
        }

           if (debug == true ){
            cout << "classifier parameters:" << endl;
                for (const auto &label : allLabels) {
                for(const auto &word : allWords){
                    if (debug == true && 
                    (postsWLpairs.find(make_pair(label, word)) 
                    != postsWLpairs.end())){ 
                        cout << "  " << label << ":" 
                        << word << ", count = " 
                        << postsWLpairs[make_pair(label,word)] 
                        << ", " << "log-likelihood = " 
                        << logLikelihood(label, word) << endl;
                    }
                
                }
            }

            }
          
        
    }


//testing function 
  void testing(const string &filename,bool debug){

        
        // Open file
        csvstream csvin(filename);

        //create a map to store row data
        map<string, string> row;

        //initialize best label
        string bestLabel; 

        set<string> trainingLabels; 

        //initialize best Pron
        double bestProb = std::numeric_limits<double>::lowest();

        //initialize count
        int accuracyCount = 0; 

        int testingCount = 0;
         
        cout<< "test data:" << endl; 
        
        while (csvin >> row) {

            ++ testingCount;

            string label = row["tag"]; 

            trainingLabels.insert(label); 

            set<string> words = unique_words(row["content"]);

            //reset best Prob
            bestProb = std::numeric_limits<double>::lowest();
    

            for (const auto &label : allLabels) {
               
                double prob = logPriorProb(label); 

                //number of training posts already exists as totalPosts

                for(const auto &word : words) {

                    prob += logLikelihood(label, word);

                }
                //******************************************************
                //find what is the correct decision if prob = bestProb
                if (prob > bestProb){
                    bestLabel = label; 
                    bestProb = prob;  
                }
                
            }


            
            cout << "  correct = " << row["tag"] << ", predicted = " 
            << bestLabel << ", log-probability score = " << bestProb << endl; 
            cout << "  content = " << row["content"] << endl << endl; 
            
    
            if (bestLabel == row["tag"]){
                    ++accuracyCount; 
                }

        }

        cout<< "performance: " << accuracyCount << " / " << 
        testingCount << " posts predicted correctly" << endl;

    }
    
};




int main(int argc, char *argv[]) {

    // initiate Classifier
    Classifier classifier;

   cout.precision(3); 
   bool debug = false;
   
   //check arg count 
   if (argc != 4 && argc!= 3){
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return -1;
   }

   //set args 
   string trainingFilename = argv[1]; 
   string testingFilename = argv[2]; 
   

    if (argc == 4){
        //check to make sure it says debug
        if (string(argv[3]) != "--debug"){ 
            cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
            return -1;
        }
        //set debug to true 
       debug = true; 
    } 
    
    try{
        csvstream csvin_train(trainingFilename);
    }
    catch(const csvstream_exception &e){
        cout << e.what() << endl;
        return 1;
    }
    
    try{
        csvstream csvin_test(testingFilename);
    }
    catch(const csvstream_exception &e){
        cout << e.what() << endl;
        return 1; 
    }

    classifier.training(argv[1],debug); 
    classifier.testing(argv[2],debug); 
 

} 


/** class Classifier {

private:

    //The total number of posts in the entire training set.
    int totalPosts; 
    //The number of unique words in the entire training set. (The vocabulary size.)
    int vocabularySize;
    //For each word w, the number of posts in the entire training set that contain it 
    map <string, int> postswWord;
    // For each label, C , the number of posts with that label.
    map <string, int> postswLabel ;
    // For each label , C , and word w, the number of posts with label C that contain w 
    map <pair<string,string>, int> postsWLpairs; 

    // 
    set<string> allLabels; 
    set<string> allWords;

    //helper functions for training 

    // EFFECTS: Return a set of unique whitespace delimited words.
    set<string> unique_words(const string &str) {
        istringstream source(str);
        set<string> words;
        string word;
        while (source >> word) {
             words.insert(word);
        }
        return words;
    }


    int countPosts(const string &filename){
        // Open file
        csvstream csvin(filename);
        //set count to 0 
        int count = 0; 
       
        map<string, string> row;
        // count rows 
         while (csvin >> row) {
             ++ count;
         }
         return count;
    }

    
    set<string> uniqueWords(const string &filename){
        // Open file
        csvstream csvin(filename);

        //create set to store unique Words
        set<string> uniqueWords;

        //create map to store row data
        map<string, string> row;

        while (csvin >> row) {
            set<string> words = unique_words(row["content"]);
            uniqueWords.insert(words.begin(), words.end());
             
        }

        return uniqueWords;

    }

     
    set<string> uniqueLabels(const string &filename){
        // Open file
        csvstream csvin(filename);

        //create set to store unique Words
        set<string> uniqueLabels;

        //create map to store row data
        map<string, string> row;

        while (csvin >> row) {
            uniqueLabels.insert(row["tag"]);
        }

        return uniqueLabels;

    }

    

    map <string, int> postsWithWord(const string &filename, const string &word){
        csvstream csvin(filename);

        //create a map to store row data
        map<string, string> row;

        //crreate a map to store the counts 
        map<string, int> postsCount;

        while (csvin >> row) {
            set<string> words = unique_words(row["content"]);
            if (words.find(word) != words.end()){
                ++ postsCount[row["tag"]];
            }
    
        }
        
        return postsCount; 

    }

    map <string, int> postsWithLabel(const string &filename, const string &label){
        csvstream csvin(filename);

        //create a map to store row data
        map<string, string> row;

        //create a map to store the counts 
        map<string, int> labelCount;


        while (csvin >> row) {

            if (row["tag"] == label){
                ++ labelCount[row["tag"]];
            }

        }

        return labelCount; 

    }

    //For each label C and word w, the number of posts with label C that contain w

    map <pair<string,string>, int> labeledPostswWord(const 
    string &filename, const string &word, const string &label){

        csvstream csvin(filename);
        
        //create a map to store row data
        map<string, string> row;

        //create a map to store the counts 
        map<pair<string,string>,int> count; 

         while (csvin >> row){
            if(label == row["tag"]){
                set<string> wordstoCheck = unique_words(row["content"]); 
                if (wordstoCheck.find(word) != wordstoCheck.end()){
                    ++ count[make_pair(label,word)];
                 }
            }

         }
        return count;
    }

    //log-proior probability 
    double logPriorProb(const string &label){
        double labelCount = static_cast<double>(postswLabel[label]);
        return log(labelCount / totalPosts);
        
    }

     
public: 
    
    // training function 

    void training(const string &filename, bool debug){
       
        csvstream csvin(filename);
        map<string, string> row;

        if (debug == true){
            cout << "training data:" << endl;
        }

        totalPosts = countPosts(filename); 
        vocabularySize = uniqueWords(filename).size(); 

        allWords = uniqueWords(filename);
        allLabels = uniqueLabels(filename);

        if(debug == true){
            //training output 
            while (csvin >> row) {
                string label = row["tag"]; 
                string content = row["content"]; 
            
                cout << "  label = " << label << ", content = " << content << endl;
            }
        }
        
        
        //find number of posts with each label
    
        for (const auto &label : allLabels) {
            postswLabel[label] = postsWithLabel(filename, label)[label];
        }

        //find number of posts with each word 
    
        for (const auto &word : allWords) {
            postswWord[word] = postsWithWord(filename, word)[word];
        }


        //find number of posts with label word pairs 
        for (const auto &label : allLabels) {
            for (const auto &word : allWords) {
                postsWLpairs[make_pair(label, word)] = 
        labeledPostswWord(filename, word, label)[make_pair(label, word)]; 
            }
        }

        cout << "trained on " << totalPosts << " examples" << endl;

        if (debug == true){
            cout << "vocabulary size = " << vocabularySize << endl<<endl; 
            cout << "classes:" << endl; 
            for (const auto &label : allLabels) {
                cout << "  "<< label << ", " << postswLabel[label] << " 
                examples, log-prior = " << logPriorProb(label) << endl;
            }
            cout << "classifier parameters:" << endl; 

        }
          
        
    }


//testing function 
  void testing(const string &filename,bool debug){

        cout<< "test data:" << endl; 
        // Open file
        csvstream csvin(filename);

        //create a map to store row data
        map<string, string> row;

        //initialize best label
        string bestLabel; 



        //initialize best Pron
        double bestProb = std::numeric_limits<double>::lowest();

        //initialize count
        int accuracyCount = 0; 

        while (csvin >> row) {

            set<string> words = unique_words(row["content"]);

            //reset best Prob
            bestProb = std::numeric_limits<double>::lowest();

            for (const auto &label : allLabels) {

                double prob = logPriorProb(label); 

                //cout << "prob" << prob << endl; 

                //number of trianing posts with this label 
                int tpostswLabel = postswLabel[label]; 

                //number of training posts already exists as totalPosts

                for(const auto &word : words) {
                    
                    //if w does not occur at all
                    if (postswWord.find(word) == postswWord.end()){
                        prob += log(1/(double)totalPosts); 
                    }

                    //if w occurs but not in that label 
                    else if(postsWLpairs.find(make_pair(label, word)) == 
                    postsWLpairs.end()){
                        prob += log((postswWord[word])/(double)totalPosts); 
                    }
                    // regular formula 
                    else{
                        int whatever = postsWLpairs[make_pair(label, word)]; 
                        cout<< whatever << endl; 
                        prob += log(postsWLpairs[make_pair(label, word)]/
                        (double)tpostswLabel);
                    }
                   
                }

                if (prob > bestProb){
                    bestLabel = label; 
                    bestProb = prob;  
                }
                
            }
            
            
            cout << "  correct = " << row["tag"] << ", predicted = " 
            << bestLabel << ", log-probability score = " << bestProb << endl; 
            cout << "  content = " << row["content"] << endl << endl; 
            
            


            if (bestLabel == row["tag"]){
                    ++accuracyCount; 
                }

        }
        cout<< "performance: " << accuracyCount<< " / " << 
        countPosts(filename) << " posts predicted correctly" << endl;
    }
    
};




int main(int argc, char *argv[]) {

    // initiate Classifier
    Classifier classifier;

   cout.precision(3); 
   bool debug = false;
   
   //check arg count 
   if (argc != 4 && argc!= 3){
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return -1;
   }

   //set args 
   string trainingFilename = argv[1]; 
   string testingFilename = argv[2]; 
   

    if (argc == 4){
        //check to make sure it says debug
        if (string(argv[3]) != "--debug"){ 
            cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
            return -1;
        }
        //set debug to true 
       debug = true; 
    } 
    

    //TRY TO OPEN FILES HERE!!!
    csvstream csvin_train(trainingFilename);
    //if (!csvin_train.is_open()){
       // cout << "Error opening file: " << trainingFilename << endl;
       // return -1; 
   // }
    

    csvstream csvin_test(testingFilename);
    //if (!csvin_test.is_open()){
   //     cout << "Error opening file: " << testingFilename << endl;
    //    return -1; 
   // }

    classifier.training(argv[1],debug); 
    classifier.testing(argv[2],debug); 
 

} **/
