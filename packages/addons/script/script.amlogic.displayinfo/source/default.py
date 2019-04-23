import xbmc, xbmcgui, xbmcaddon

ADDON        = xbmcaddon.Addon()
ADDONID      = ADDON.getAddonInfo('id')
ADDONVERSION = ADDON.getAddonInfo('version')

def log(txt):
  if isinstance (txt,str):
    txt = txt.decode("utf-8")
    message = u'%s: %s' % (ADDONID, txt)
    xbmc.log(msg=message.encode("utf-8"), level=xbmc.LOGDEBUG)

if (__name__ == "__main__"):
  log('script version %s started' % ADDONVERSION)

  window = xbmcgui.Window(10000)

  try:
    with open("/sys/class/amhdmitx/amhdmitx0/config") as fp:
      for line in fp:
        if ":" in line:
          parts = line.split(":")
          log("Parsing %s as %s" %(parts[0], parts[1]))
          if parts[0].strip() == "VIC":
            window.setProperty("amlogic.hdmitx.displaymode", parts[1].strip().split(" ")[1])
          if parts[0].strip() == "Colour depth":
            window.setProperty("amlogic.hdmitx.colourdepth", parts[1].strip())
          if parts[0].strip() == "Colourspace":
            window.setProperty("amlogic.hdmitx.colourspace", parts[1].strip())
          if parts[0].strip() == "Colour range":
            window.setProperty("amlogic.hdmitx.colourrange", parts[1].strip())
          if parts[0].strip() == "EOTF":
            window.setProperty("amlogic.hdmitx.eotf", parts[1].strip())
          if parts[0].strip() == "Colourimetry":
            window.setProperty("amlogic.hdmitx.colourimetry", parts[1].strip())
  except IOError:
    window.setProperty("amlogic.hdmitx.displaymode", "N/A")
    window.setProperty("amlogic.hdmitx.colourdepth", "N/A")
    window.setProperty("amlogic.hdmitx.colourspace", "N/A")
    window.setProperty("amlogic.hdmitx.colourrange", "N/A")
    window.setProperty("amlogic.hdmitx.eotf", "N/A")
    window.setProperty("amlogic.hdmitx.colourimetry", "N/A")
