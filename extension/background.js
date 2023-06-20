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

const scanTabs = () => {
  chrome.tabs.query({}, (tabs) => {
    const hasBlackListed = tabs.some((tab) => isTabBlackListed(tab, true))

    chrome.storage.sync.get("hasBlackListed", (data) => {
      if (data.hasBlackListed === hasBlackListed) return

      chrome.storage.sync.set({ hasBlackListed }, () => {
        if (hasBlackListed) {
          turnShocker("on")
        } else {
          turnShocker("off")
        }
      })
    })
      
  })
}

const isTabBlackListed = (tab) => {
  if (!tab.url) return

  let isBlackListed = false
  blackList.forEach((blackListedUrl) => {
    if (tab.url.includes(blackListedUrl)) {
      isBlackListed = true
    }
  })

  return isBlackListed
}

// Listen for when the tab updates
chrome.tabs.onUpdated.addListener((tabId, changeInfo, tab) => {
    scanTabs()
});

// Listen for when active tab changes
chrome.tabs.onActivated.addListener((activeInfo) => {
    scanTabs()
});

// Listen for tab close
chrome.tabs.onRemoved.addListener((tabId, removeInfo) => {
  scanTabs()
})
