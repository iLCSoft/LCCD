#####################################################
#
# Set the environment for LCCD - modify as needed
#
# F.Gaede, DESY
# 01-02-2005
#
#####################################################

# -- required
#export LCIO=/opt/products/lcio/1.3.1
export LCIO=~gaede/lcio/devel/v01-04beta

#-- comment out for production 
export LCCDDEBUG 1

#-- optionally build with CondDBMySQL support
export CondDBMySQL=/data/gaede/CondDBMySQL

export MYSQL_PATH=/opt/products/mysql/3.23.58



