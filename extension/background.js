// Listen for when the extension is installed or updated.
chrome.runtime.onInstalled.addListener(() => {
  console.log('Post installed!');
});

const blackList = [
  "tiktok.com",
  "youtube.com"
]

const ARDUINO_INTERFACE_SERVER = "http://localhost:5000"

const turnShocker = (state) => {
    fetch(ARDUINO_INTERFACE_SERVER + "/shocker/" + state)
      .catch((error) => {
        console.log(error)
      })
}


const handleTab = (tab) => {
  if (!tab.url) return

  let isBlackListed = false
  blackList.forEach((blackListedUrl) => {
    if (tab.url.includes(blackListedUrl)) {
      isBlackListed = true
    }
  })

  if (!isBlackListed) {
    turnShocker("off")
    return
  }
  
  console.log("Get back to work buddy")
  turnShocker("on")
}

// Listen for when the tab updates
chrome.tabs.onUpdated.addListener((tabId, changeInfo, tab) => {
    if (changeInfo.status === 'complete' ) {
      handleTab(tab)
    }
});

// Listen for when active tab changes
chrome.tabs.onActivated.addListener((activeInfo) => {
    chrome.tabs.get(activeInfo.tabId, (tab) => {
      handleTab(tab)
    });
});