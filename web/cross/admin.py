#!/usr/bin/python2
import sqlite3
import time
from selenium import webdriver
from selenium.webdriver.chrome.options import Options 

con = sqlite3.connect('/var/www/cross/db.db')
cur = con.cursor()
opt = Options()
opt.add_argument('--headless')
opt.add_argument('--no-sandbox')
driver = webdriver.Chrome("/usr/bin/chromedriver", chrome_options=opt)

URL = "http://127.0.0.1"

def login():
    print("LOGGING IN")
    driver.get(URL + "/login.php")
    element = driver.find_element_by_name("username")
    element.send_keys("admin")
    element = driver.find_element_by_name("password")
    element.send_keys("X90uOfniDEBoC7i60Fcj")

    element.submit()

def get_msg(ID):
    print("GETTING MSG %d" % ID)
    driver.get(URL + "/index.php?id=%d" % ID)

def check_db(last):
    cur.execute("SELECT rowid FROM reports WHERE rowid > %d" %last)
    l = []
    for x in cur.fetchall():
        l.append(x[0])
    return l

last = -1
login()

while(True):
    l = check_db(last)
    if(l):
        last = max(l)
        for x in l:
            get_msg(x)
            #in case payload deleted cookies
            if("Message could not be displayed." in driver.page_source or
            "Please log in or register to send messages." in driver.page_source):
                print("MSG COULD NOT BE DISPLAYED")
                driver.delete_all_cookies()
                login()
                get_msg(x)
            #in case payload opens new windows
            while(len(driver.window_handles) > 1):
                print("ADDITIONAL WINDOWS OPEN")
                driver.switch_to_window(driver.window_handles[1])
                driver.close()
                driver.switch_to_window(driver.window_handles[0])
    print("SLEEPING")
    time.sleep(5)
