# ufw_ultifi_helloworld

1. Clone workspace with initialization script:
```git clone https://github.com/GM-SDV-UP/gmultifi_ufw_workspace.git uspace-helloworld; cd uspace-helloworld;```
2. Run init_ultifi_repo.sh initialization script by passing the manifest file as parameter:
```./init_ultifi_repo.sh -m gmultifi_pvm_ufw_helloworld.xml```
3. step into uDev container using uBox:
```cd ultifi```
```../ubox.sh```
4. Build native on host:
Shared Objects:
```source ./build.sh -r -s```
Static libraries + rebuild:
```source ./build.sh -r -rebuild-yes```
5. Build aarch64 (cross) on host:
Shared Objects:
```source ./build.sh -r -cross -s```
Static libraries + rebuild:
```source ./build.sh -r -cross -rebuild-yes```
6. create 'helloworld' conan package:
```source ./build.sh -r -cross -s -install -strip -rebuild-yes -conan x.y.z```
