import bluetooth

def search():
   while True:
      devices = bluetooth.discover_devices(lookup_names = True)

      yield devices

for addr, name in search():
   print "{0} - {1}".format(addr, name)
