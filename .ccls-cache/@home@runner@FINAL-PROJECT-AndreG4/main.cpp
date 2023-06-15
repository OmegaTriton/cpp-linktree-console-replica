#include <iostream>
#include <fstream>
#include <vector>
#include <sys/stat.h> //mkdir()
#include <cstdio> //remove()
#include <cstdlib> //system("clear")
#include "cin_fail_clear.h" //cin_fail_clear()
#include "kbhit.h"
using namespace std;

class Link{
public:
  Link(string plat, string lin){
    platform = plat;
    link = lin;
  }
  string get_platform() const{
    return platform;
  }
  string get_link() const{
    return link;
  }
  void set_platform(string plat){
    platform = plat;
  }
  void set_link(string lin){
    link = lin;
  }
private:
  string platform, link;
};
class Profile{
public:
  string get_name() const{
    return name;
  }
  string get_platform(int index) const{
    return links[index].get_platform();
  }
  string get_link(int index) const{
    return links[index].get_link();
  }
  int get_links_size() const{
    return links.size();
  }
  void add_link(Link link){
    links.push_back(link);
  }
  void edit_link(string platform, bool plat, bool lin, string new_plat, string new_link){
    int index = find_link_index(platform);
    if(index == -1) return;
    if(plat)
      links[index].set_platform(new_plat);
    if(lin)
      links[index].set_link(new_link);
  }
  void delete_link(string platform){
    int index = find_link_index(platform);
    if(index == -1) return;
    for(int i = index + 1; i < links.size(); i++)
      links[i-1] = links[i];
    links.pop_back();
  }
  void show_links() const{
    if(links.empty()){
      cout << "There are no links in profile: " << name << endl;
      return;
    }
    cout << "Link(s) of: " << name << "\n";
    for(int i = 0; i < links.size(); i++)
      cout << links[i].get_platform() << " -- " << links[i].get_link() << endl;
  }
  void set_name(string nam){
    name = nam;
  }
  bool platform_exists(string platform) const{
    if(find_link_index(platform) != -1)
      return true;
    return false;
  }
  int find_link_index(string platform) const{
    for(int i = 0; i < links.size(); i++)
      if(links[i].get_platform() == platform)
        return i;
    return -1;//error code
  }
private:
  string name;
  vector<Link> links;
};

void read(vector<Profile>& profiles, const char folder_name[]){
  const string folder_nam(folder_name);
  string prof_name;
  fstream names;
  while(!names.is_open()) names.open(folder_nam + "/Names.txt", ios::in);
  while(getline(names, prof_name)){
    fstream profile_file;
    string line;
    Profile profile;
    profile.set_name(prof_name);
    while(!profile_file.is_open()) profile_file.open(folder_nam + "/" + prof_name + ".txt", ios::in);
    while(getline(profile_file, line)){
      int breaker = line.find(" -- ");
      string platform = line.substr(0, breaker);
      string lin = line.substr(breaker+4);
      Link link(platform, lin);
      profile.add_link(link);
    }
    profiles.push_back(profile);
    while(profile_file.is_open()) profile_file.close();
  }
  while(names.is_open()) names.close();
}
void save(vector<Profile>& profiles, const char folder_name[]){
  const string folder_nam(folder_name);
  mkdir(folder_name, 0777);
  fstream names;
  while(!names.is_open()) names.open(folder_nam + "/Names.txt", ios::out);
  for(int i = 0; i < profiles.size(); i++){
    fstream profile;
    string prof_name = profiles[i].get_name();
    names << prof_name;
    if(i+1 != profiles.size()) names << endl;
    while(!profile.is_open()) profile.open(folder_nam + "/" +  prof_name + ".txt", ios::out);
    for(int j = 0; j < profiles[i].get_links_size(); j++){
      profile << profiles[i].get_platform(j) << " -- " << profiles[i].get_link(j);
      if(j+1 != profiles[i].get_links_size()) profile << endl;
    }
    while(profile.is_open()) profile.close();
  }
  while(names.is_open()) names.close();
}
int find_prof_name(vector<Profile>& profiles, string name){
  for(int i = 0; i < profiles.size(); i++)
    if(profiles[i].get_name() == name)
      return i;
  return -1;//error code
}
void delete_profile(vector<Profile>& profiles, string name, const char folder_name[]){
  const string folder_nam(folder_name);
  int index = find_prof_name(profiles, name);
  if(index == -1) return;
  for(int i = index + 1; i < profiles.size(); i++)
    profiles[i-1] = profiles[i];
  profiles.pop_back();
  remove((folder_nam + "/" + name + ".txt").c_str());
}
void show_all(vector<Profile>& profiles){
  for(int i = 0; i < profiles.size(); i++){
    profiles[i].show_links();
    cout << endl;}
}
bool profile_name_exists(vector<Profile>& profiles, string name){
  for(int i = 0; i < profiles.size(); i++)
    if(profiles[i].get_name() == name)
      return true;
  return false;
}

int main(){
  vector<Profile> profiles;
  const char folder_name[] = "Profiles";
  const string folder_nam{folder_name};
  mkdir(folder_name, 0777);
  fstream names;
  while(!names.is_open()) names.open(folder_nam + "/Names.txt", ios::app);
  while(names.is_open()) names.close();
  int random_thing = 0;
  read(profiles, folder_name);
  bool running = true;
  cout << "This program is a database of profiles that contain links to platforms.\n";
  cout << "You may create, edit, delete, or view profiles.\n";
  cout << "If you terminate the program, any changes made will not be saved.\n";
  cout << "Progam must be closed & saved by saying 'n' to switch moded or by entering SAVE&EXIT in available modes\n";
  cout << "This text will not show again until the program is run again\n\n";
  cout << "Press any key to begin...\n";
  while(!kbhit()) {}
  while(running){
    string current_mode;
    do{
    system("clear");
    cout << "Available modes: CREATE EDIT DELETE VIEW SAVE&EXIT\n";
    cout << "Enter mode: ";
    cin >> current_mode;
    if(random_thing == 0){
      random_thing++;
      current_mode = current_mode.substr(1);
    }}
    while(!(current_mode == "CREATE" || current_mode == "EDIT" || current_mode == "DELETE" || current_mode == "VIEW" || current_mode == "SAVE&EXIT"));
    system("clear");
    bool mode_run = true;

    if(current_mode == "SAVE&EXIT") break;
    else if(current_mode == "CREATE"){
      while(mode_run){
        string prof_name = "", platform, lin;
        Profile profile;
        bool first = true;
        cin_fail_clear();
        do{
        system("clear");
        if(prof_name != "" && prof_name != "Names")
          cout << prof_name << " already exists\n";
        if(!first && (prof_name == "" || prof_name == "Names"))
          cout << "Invalid input\n";
        cout << "Enter Profile name: ";
        getline(cin, prof_name);
        first = false;}
        while(profile_name_exists(profiles, prof_name) || prof_name == "" || prof_name == "Names");
        profile.set_name(prof_name);
        system("clear");
        cout << "Profile: " << prof_name;
        cout << "\nOnce finished adding links, enter 'STOP' in the platform section\n";
        do{
        cout << "\nEnter Platform: ";
        cin >> platform;
        if(platform != "STOP"){
          cout << "Enter Link to " << platform << ": ";
          cin >> lin;
          Link link(platform, lin);
          profile.add_link(link);}
        }
        while(platform != "STOP");
        system("clear");
        profiles.push_back(profile);
        cout << "Profile created!\nPress any key to continue...\n";
        while(!kbhit()) {}
        char cont;
        do{
        system("clear");
        cout << "Create a new profile?(y/n): ";
        cin >> cont;}
        while(!(cont == 'y' || cont == 'n'));
        if(cont == 'n')
          mode_run = false;
      }
    }
    else if(current_mode == "EDIT"){
      while(mode_run){
        if(profiles.size() == 0){
          cout << "Currently there are 0 profiles\nPress any key to continue...\n";
          while(!kbhit()) {}
          break;
        }
        string prof_name = "";
        int prof_index;
        bool edit_run = true;
        cin_fail_clear();
        do{ //gets valid profile name
        system("clear");
        if(prof_name != "")
          cout << prof_name << " does not exist\n";
        cout << "Profiles:\n";
        for(int i = 0; i < profiles.size(); i++)
          cout << profiles[i].get_name() << endl;
        cout << "Enter Profile Name(to edit): ";
        getline(cin, prof_name);}
        while(!profile_name_exists(profiles, prof_name));
        prof_index = find_prof_name(profiles, prof_name);
        system("clear");
        while(edit_run){
          prof_name = profiles[prof_index].get_name();
          string edit_mode;
          do{
          system("clear");
          cout << "Profile: " << prof_name;
          cout << "\nEdit Modes: NAME PLATFORM_OF_LINK LINK_OF_LINK ADD_LINK\n";
          cout << "Enter Mode: ";
          cin >> edit_mode;}
          while(!(edit_mode == "NAME" || edit_mode == "PLATFORM_OF_LINK" || edit_mode == "LINK_OF_LINK" || edit_mode == "ADD_LINK"));
          if(edit_mode == "NAME"){
            string new_name;
            system("clear");
            cout << "Caution: This change will not be reverted by terminating the program...\n";
            cout << "Profile: " << prof_name;
            cout << "\nEnter new name: ";
            cin_fail_clear();
            getline(cin, new_name);
            profiles[prof_index].set_name(new_name);
            cout << endl << prof_name << " has been set to " << profiles[prof_index].get_name() << endl;
            rename((folder_nam + "/" + prof_name + ".txt").c_str(), (folder_nam + "/"+ profiles[prof_index].get_name() + ".txt").c_str());
            fstream names;
            while(!names.is_open()) names.open(folder_nam + "/" +"Names.txt", ios::out);
            for(int i = 0; i < profiles.size(); i++){
              names << profiles[i].get_name();
              if(i+1 != profiles.size()) names << endl;
            }
            while(names.is_open()) names.close();
          }
          else if(edit_mode == "PLATFORM_OF_LINK"){
            string old_platform, new_platform;
            do{
            cout << "Enter old platform: ";
            cin >> old_platform;}
            while(!profiles[prof_index].platform_exists(old_platform));
            cout << "Enter new platform: ";
            cin >> new_platform;
            profiles[prof_index].edit_link(old_platform, true, false, new_platform, "");
            cout << old_platform << " has been set to " << profiles[prof_index].get_platform(profiles[prof_index].find_link_index(new_platform)) << endl;
          }
          else if(edit_mode == "LINK_OF_LINK"){
            string platform, new_link;
            do{
            cout << "Enter platform: ";
            cin >> platform;}
            while(!profiles[prof_index].platform_exists(platform));
            cout << "Enter new link: ";
            cin >> new_link;
            profiles[prof_index].edit_link(platform, false, true, "", new_link);
            cout << platform << "'s link has been set to " << profiles[prof_index].get_link(profiles[prof_index].find_link_index(platform)) << endl;
          }
          else if(edit_mode == "ADD_LINK"){
            string platform, lin;
            Profile* profile = &profiles[find_prof_name(profiles, prof_name)];
            system("clear");
            cout << "Profile: " << prof_name;
            cout << "\nOnce finished adding links, enter 'STOP' in the platform section\n(Case Sensitive -- Capital first letter of platform)\n";
            do{
            cout << "\nEnter Platform: ";
            cin >> platform;
            if(profile->platform_exists(platform))
              cout << platform << " already exists in this profile";
            if(platform != "STOP" && !profile->platform_exists(platform)){
              cout << "Enter Link to " << platform << ": ";
              cin >> lin;
              Link link(platform, lin);
              profile->add_link(link);}
            }
            while(platform != "STOP");
            cout << "Links were added to " << prof_name;
          }
          cout << "\nPress any key to continue...\n";
          while(!kbhit()) {}
          char edit_again;
          do{
          system("clear");
          cout << "Edit same profile again?(y/n): ";
          cin >> edit_again;}
          while(!(edit_again == 'y' || edit_again == 'n'));
          if(edit_again == 'n')
            edit_run = false;
        }
        char mode_again;
        do{
        system("clear");
        cout << "Stay in EDIT mode?(y/n): ";
        cin >> mode_again;}
        while(!(mode_again == 'y' || mode_again == 'n'));
        if(mode_again == 'n')
          mode_run = false;
        system("clear");
      }
    }
    else if(current_mode == "DELETE"){
      while(mode_run){
        if(profiles.size() == 0){
          system("clear");
          cout << "Currently there are 0 profiles\nPress any key to continue...\n";
          while(!kbhit()) {}
          break;
        }
        string prof_name = "", delete_mode;
        int prof_index;
        bool first = true;
        cin_fail_clear();
        do{ //gets valid profile name
        system("clear");
        if(prof_name != "" && prof_name != "Names")
          cout << prof_name << " does not exist\n";
        if(!first && (prof_name == "" || prof_name == "Names"))
          cout << "Invalid input\n";
        cout << "Profiles:\n";
        for(int i = 0; i < profiles.size(); i++)
          cout << profiles[i].get_name() << endl;
        cout << "Enter Profile Name: ";
        getline(cin, prof_name);
        first = false;}
        while(!profile_name_exists(profiles, prof_name) || prof_name == "" || prof_name == "Names");
        prof_index = find_prof_name(profiles, prof_name);
        do{
        system("clear");
        cout << "CAUTION: If PROFILE is selected, it cannot be undone\n";
        cout << "Profile: " << prof_name;
        cout << "\nDelete: PROFILE LINK\nEnter: ";
        cin >> delete_mode;}
        while(!(delete_mode == "PROFILE" || delete_mode == "LINK"));
        
        system("clear");
        if(delete_mode == "PROFILE"){
          delete_profile(profiles, prof_name, folder_name);
          cout << "Deletion of " << prof_name << " completed";
          cout << "\nPress any key to continue...\n";
          while(!kbhit()){}
        }
        else if(delete_mode == "LINK"){
          Profile* profile = &profiles[prof_index];
          string platform = "";
          cout << "Profile: " << prof_name << endl;
          if(profile->get_links_size() != 0){
            profile->show_links();
            cout << endl;
            do{
            if(profile->platform_exists(platform) && platform != "")
          cout << prof_name << " does not exist\n";
            cout << "Platform of Link: ";
            cin >> platform;}
            while(!profile->platform_exists(platform));
            profile->delete_link(platform);
            system("clear");
            cout << platform << " link deleted";
            cout << "\nPress any key to continue...\n";
            while(!kbhit()){}
          }
          else{
            cout << "There are no links in this profile";
            cout << "\nPress any key to continue..." << endl;
            while(!kbhit()){}
          }
        }
        char del_again;
        do{
        system("clear");
        cout << "Delete Again?(y/n): ";
        cin >> del_again;
        }
        while(!(del_again == 'y' || del_again == 'n'));
        if(del_again == 'n')
          mode_run = false;
      }
    }
    else if(current_mode == "VIEW"){
      while(mode_run){
        if(profiles.size() == 0){
          cout << "Currently there are 0 profiles\nPress any key to continue...\n";
          while(!kbhit()) {}
          break;
        }
        string view_mode;
        system("clear");
        do{
        cout << "View Modes: NAMES LINKS\nEnter Mode: ";
        cin >> view_mode;
        }
        while(!(view_mode == "NAMES" || view_mode == "LINKS"));
        if(view_mode == "NAMES"){
          system("clear");
          cout << "Profiles:\n";
          for(int i = 0; i < profiles.size(); i++){
            cout << profiles[i].get_name();
            if(i+1 != profiles.size())
              cout << endl;
          }
        }
        else if(view_mode == "LINKS"){
          string amount;
          do{
          system("clear");
          cout << "AMOUNT: ONE ALL\nEnter: ";
          cin >> amount;}
          while(!(amount == "ONE" || amount == "ALL"));
          if(amount == "ONE"){
            string prof_name = "";
            int prof_index;
            cin_fail_clear();
            do{
            system("clear");
            if(prof_name != "")
              cout << prof_name << " does not exist\n";
            cout << "Enter profile: ";
            getline(cin, prof_name);}
            while(!profile_name_exists(profiles, prof_name));
            system("clear");
            prof_index = find_prof_name(profiles, prof_name);
            profiles[prof_index].show_links();
          }
          else if(amount == "ALL"){
            system("clear");
            show_all(profiles);
          }
        }
        cout << "\nPress Enter to continue...\n";
        while(!kbhit()){}
        char view_again;
        cin_fail_clear();
        do{
        system("clear");
        cout << "View Again?(y/n): ";
        cin >> view_again;
        }
        while(!(view_again == 'y' || view_again == 'n'));
        if(view_again == 'n')
          mode_run = false;
      }
    }
    char cont;
    do{
    system("clear");
    cout << "Switch mode?(y/n): ";
    cin >> cont;}
    while(!(cont == 'y' || cont == 'n'));
    if(cont == 'n')
      running = false;
    system("clear");
  }
  save(profiles, folder_name);
  return 0;
}