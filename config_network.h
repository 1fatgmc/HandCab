// enter the SSIDs and passwords of as many networks you wish to try to connect to.
const int maxSsids = 1;

// There must be the same number of entities (specified above) in each of the following arrays

const String ssids[maxSsids] = {"RPi-JMRI"};
const String passwords[maxSsids] = {"rpI-jmri"};

const String turnoutPrefixes[maxSsids] = {""};

// const String turnoutPrefixes[maxSsids] = {"NT"};  // required if you wish to use turnouts  
// this the prefix of all turnout system names for YOUR system, for the wiThrottle servers on the the networks above

const String routePrefixes[maxSsids] = {""};

// const String routePrefixes[maxSsids] = {"IO:AUTO:"};  // required if you wish to use routes  
// this is the prefix of all route system names for YOUR system, for the wiThrottle servers on the  networks above


