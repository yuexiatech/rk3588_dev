DEB_BUILD_OPTIONS="parallel=4 nocheck" dpkg-buildpackage -a arm64
cmake -DRKPLATFORM=ON -DHAVE_DRM=ON && make