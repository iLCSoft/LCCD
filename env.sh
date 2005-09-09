#####################################################
#
# Set the environment for LCCD - modify as needed
#
# F.Gaede, DESY
# 01-02-2005
#
#####################################################

# -- required
export LCIO=/afs/desy.de/group/it/ilcsoft/lcio/v01-05

#-- comment out for production 
export LCCDDEBUG=1

#-- optionally build with CondDBMySQL support
export CondDBMySQL=/afs/desy.de/group/it/ilcsoft/CondDBMySQL
export MYSQL_PATH=/opt/products/mysql/3.23.58
export LD_LIBRARY_PATH=$CondDBMySQL/lib


#----- for documetation only
export LCCDVERSION=v00-03