tar xvf ../sources/podman-bin/podman-bin-5.0.0.tar.gz
mv podman-5.0.0 a
tar xvf ../sources/podman-bin/podman-bin-5.0.0.tar.gz
mv podman-5.0.0 b

cd b
find . -name "*.go" -print | \
  xargs sed -i \
    -e '/^\W*\/\// ! s#/etc/containers#/storage/.kodi/addons/service.system.podman/etc/containers#g' \
    -e '/^\W*\/\// ! s#/usr/share/containers#/storage/.kodi/userdata/addon_data/service.system.podman/podman/etc/containers#g' \
    -e '/^\W*\/\// ! s#/var/lib/containers#/storage/.kodi/userdata/addon_data/service.system.podman/podman#g'

cd ..
diff -Nur a b > podman-0002-path-changes.patch
