# Configuring HDFS and Running WordCount

## User, SSH and Java Configuration

The following configuration should be done on every machine.

```
sudo addgroup hadoop_
sudo adduser --ingroup hadoop_ hduser_ # Team, 1, 1, 1, 1
sudo adduser hduser_ sudo
su - hduser_
ssh-keygen -t rsa -P ""
```

Copy each `$HOME/.ssh/id_rsa.pub` to `$HOME/.ssh/authorized_keys` of other machines.

```
sudo apt-get update
sudo apt-get install openssh-server default-jdk
```

## Hadoop Configuration

```
wget http://mirrors.sonic.net/apache/hadoop/common/hadoop-2.9.2/hadoop-2.9.2.tar.gz
sudo tar xzf hadoop-2.9.2.tar.gz
sudo mv hadoop-2.9.2 hadoop
sudo chown -R hduser_:hadoop_ hadoop
readlink -f /usr/bin/java | sed "s:bin/java::" # get java home path
```

### bashrc
Add the following to `~/.bashrc`:
```
#Set HADOOP_HOME
export HADOOP_HOME=/home/hduser_/hadoop
#Set JAVA_HOME
export JAVA_HOME=<Installation Directory of Java>
# Add bin/ directory of Hadoop to PATH
export PATH=$PATH:$HADOOP_HOME/bin
```
```
source ~/.bashrc
```

### hadoop-env.sh
Set `JAVA_HOME` inside file `$HADOOP_HOME/etc/hadoop/hadoop-env.sh`

### 
