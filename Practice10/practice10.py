import csv
import random
import matplotlib.pyplot as plt

time = 0

y = [0,0,0,0,0,0,0]
ax = ["Delete","Permissions","Update (Overwrite)","Append","Read","Knowledge","None"]

class User:
    username = ""
    password = ""
    access_level = 0
    def __init__(self,username,password,access_level):
        self.username = username
        self.password = password
        self.access_level = access_level

class Data:
    name = ""
    content = ""
    access_level = 0
    owner = ""
    
    def __init__(self,name,content,access_level,owner):
        self.name = name
        self.content = content
        self.access_level = access_level
        self.owner = owner
    
    def __str__(self):
        return "File name: " + str(self.name) + " - Owner: "+self.owner+" - Access: " + str(self.access_level)
    
    def fileappend(self,user):
        if self.owner == user.username:
            print("Hi! "+self.owner) 
        elif user.access_level < self.access_level:
            print("You don't have permission to write in this file")
            return
        print()
        print("File content: ")
        print()
        print(self.content)
        print()
        print("Type the content to append to the file: ")
        s = input()
        self.content += s
    
    def update(self,user):
        if self.owner == user.username:
            print("Hi! "+self.owner)
        elif user.access_level < self.access_level:
            print("You don't have permission to overwrite this file")
            return
        
        print()
        print("Are you sure to overwrite the file?")
        print("1: YES 2: NO")
        ow = int(input())
        if ow == 1:
            print()
            print("Type the new content of the file: ")
            s = input()
            self.content = s
            print("The new file content is: ")
            print()
            print(self.content)
        else:
            print("Returning to menu...")
            print()
            return
        
    def changeAccess(self,user):
        if self.owner == user.username:
            print("Hi! "+self.owner)
        elif user.access_level < self.access_level:
            print("You don't have permission to change the access of this file")
            return
        print("Type the new access level ")
        print()
        print("[7] - Only you and the admins can see this file")
        print("[6] - Any user can see this file but no one can open it")
        print("[5] - Any user can open this file")
        print("[4] - Any user can append in this file")
        print("[3] - Any user can overwrite this file")
        print("[2] - Any change the permissions of this file")
        print("[1] - Any user can delete this file")
        
        per = int(input())
        
        if(per < 1 or per > 7):
            print("That is not a valid permission")
            return
        
        self.access_level = per
    
    def open(self, user):
        if self.owner == user.username:
            print("Hi! "+self.owner)
        elif user.access_level < self.access_level:
            print("You don't have permission to change the access of this file")
            return
        option = 0
        if(self.owner == user.username):
            print()
        elif user.access_level < self.access_level:
            return
        print("Content:")
        print()
        print(self.content)       
        print()
        print("------- "+self.name+ " --------")
        print()
        print("Operations: ")
        print()
        print("[1] - Append")
        print("[2] - Update (Overwrite)")
        print("[3] - Change permissions")
        print("[4] - Close")
        
        
        option = int(input())
        if option == 1:
            self.fileappend(user)
        elif option == 2:
            self.update(user)
        elif option == 3:
            self.changeAccess(user)
        elif option == 4:
            return

class BTreeNode:
    def __init__(self,t,leaf):
        self.t = t
        self.keys = [None] * (2 * t - 1)
        self.C = [None] * (2 * t)
        self.n = 0
        self.leaf = leaf

    def insertNonFull(self,data):
        i = self.n - 1 
        if self.leaf: 
            while i >= 0 and self.keys[i].name > data.name: 
                self.keys[i+1] = self.keys[i] 
                i-=1 
            self.keys[i + 1] = data 
            self.n += 1 
        else: 
            while i >= 0 and self.keys[i].name > data.name: 
                i-=1 
            if self.C[i+1].n == 2 * self.t - 1:
                self.splitChild(i+1, self.C[i+1]) 
                if self.keys[i+1].name < data.name:
                    i+=1 
            self.C[i + 1].insertNonFull(data) 
    

    def splitChild(self,i,y):
        z = BTreeNode(y.t, y.leaf) 
        z.n = self.t - 1 
        for j in range (self.t-1): 
            z.keys[j] = y.keys[j + self.t]
        if not y.leaf: 
            for j in range(self.t): 
                z.C[j] = y.C[j + self.t]
        y.n = self.t -1
        
        for j in range(self.n, i , -1): 
            self.C[j + 1] = self.C[j] 
        
        self.C[i+1] = z
        for j in range(self.n -1,i-1, -1): 
            self.keys[j+1] = self.keys[j] 
            
        self.keys[i] = y.keys[self.t - 1] 
        self.n += 1 
    
    def traverse(self,l,user):
        for i in range(self.n):
            if not self.leaf: 
                self.C[i].traverse(l+1,user)
            if self.keys[i].name == user.username:
                print("\t"*l,l,self.keys[i],end = ' ')
            elif self.keys[i].access_level > user.access_level:
                print("\t"*l,"Hidden file")
            else:
                print("\t"*l,l,self.keys[i],end = ' ')
        print() 
        if not self.leaf: 
            self.C[i + 1].traverse(l+1,user) 
            
    def traverseStats(self,l):
        global y
        for i in range(self.n):
            y[self.keys[i].access_level-1] += 1
            if not self.leaf: 
                self.C[i].traverseStats(l+1)
            #print("\t"*l,l,self.keys[i],end = ' ')
        #print() 
        if not self.leaf: 
            self.C[i + 1].traverseStats(l+1) 
    
    def traverseSave(self,l,toSave):
        for i in range(self.n):
            if not self.leaf: 
                self.C[i].traverseSave(l+1,toSave)
            #print("\t"*l,l,self.keys[i],end = ' ')
            toSave.append(self.keys[i])
        #print() 
        if not self.leaf: 
            self.C[i + 1].traverseSave(l+1,toSave) 
        
    def search(self,k):
        global time
        time +=1
        i = 0 
        while i < self.n and k > self.keys[i].name: 
            i+=1 
        if i < self.n and k == self.keys[i].name:
            print("Time: "+str(time))
            time = 0
            return self.keys[i] 
        if self.leaf:
            return None
        return self.C[i].search(k)
     
    def delete(self, key):
        i = 0
        while i < self.n and key > self.keys[i].name:
            i += 1

        if i < self.n and key == self.keys[i].name:
            if self.leaf:
                # Case 1: Key is in this node and the node is a leaf.
                self.deleteFromLeaf(i)
            else:
                # Case 2: Key is in this node and the node is not a leaf.
                self.deleteFromNonLeaf(i)
        else:
            # Key is not in this node, find the appropriate child.
            if self.leaf:
                # Key is not in the tree.
                return

            # Determine if the child where the key could be exists.
            childExists = (i < self.n)

            if self.C[i].n < self.t:
                # Case 3a: If the child has less than t keys, fill it.
                self.fillChild(i)

            if childExists and i > self.n:
                # Key may be in the last child, so we must search there.
                self.C[i].delete(key)
            else:
                # Recurse to the appropriate child.
                self.C[i].delete(key)

    def deleteFromLeaf(self, i):
        # Case 1: Remove the key from this node.
        for j in range(i, self.n - 1):
            self.keys[j] = self.keys[j + 1]
        self.n -= 1

    def deleteFromNonLeaf(self, i):
        key = self.keys[i]
        if self.C[i].n >= self.t:
            # Case 2a: If the child has at least t keys, find the predecessor.
            pred = self.getPredecessor(i)
            self.keys[i] = pred
            self.C[i].delete(pred)
        else:
            if self.C[i + 1].n >= self.t:
                # Case 2b: If the child has less than t keys but has an immediate sibling with at least t keys.
                succ = self.getSuccessor(i)
                self.keys[i] = succ
                self.C[i + 1].delete(succ)
            else:
                # Case 2c: If both the child and its immediate sibling have t-1 keys, merge the child with the sibling.
                self.mergeChild(i)

    def getPredecessor(self, i):
        # Go to the rightmost (maximum) child of the i-th child.
        current = self.C[i]
        while not current.leaf:
            current = current.C[current.n]
        return current.keys[current.n - 1]

    def getSuccessor(self, i):
        # Go to the leftmost (minimum) child of the (i+1)-th child.
        current = self.C[i + 1]
        while not current.leaf:
            current = current.C[0]
        return current.keys[0]

    def fillChild(self, i):
        # Case 3b: If the child has t-1 keys but an immediate sibling has t or more keys, borrow a key.
        if i != 0 and self.C[i - 1].n >= self.t:
            self.borrowFromPrev(i)
        elif i != self.n and self.C[i + 1].n >= self.t:
            self.borrowFromNext(i)
        else:
            # Case 3c: If both the child and its immediate siblings have t-1 keys, merge with a sibling.
            if i != self.n:
                self.mergeChild(i)
            else:
                self.mergeChild(i - 1)

    def borrowFromPrev(self, i):
        child = self.C[i]
        sibling = self.C[i - 1]

        # Shift all keys in the child to the right.
        for j in range(child.n, 0, -1):
            child.keys[j] = child.keys[j - 1]

        if not child.leaf:
            # Shift all child's children pointers to the right.
            for j in range(child.n + 1, 0, -1):
                child.C[j] = child.C[j - 1]

        # Set child's first key to the parent's key (self.keys[i-1]).
        child.keys[0] = self.keys[i - 1]

        # Move the last key from the sibling to the parent.
        self.keys[i - 1] = sibling.keys[sibling.n - 1]

        child.n += 1
        sibling.n -= 1

    def borrowFromNext(self, i):
        child = self.C[i]
        sibling = self.C[i + 1]

        # Set child's last key to the parent's key (self.keys[i]).
        child.keys[child.n] = self.keys[i]

        if not child.leaf:
            # Copy the first key from the sibling to the parent.
            child.C[child.n + 1] = sibling.C[0]
            self.keys[i] = sibling.keys[0]

        # Shift keys in the sibling.
        for j in range(1, sibling.n):
            sibling.keys[j - 1] = sibling.keys[j]

        if not sibling.leaf:
            # Shift the child pointers in the sibling.
            for j in range(1, sibling.n + 1):
                sibling.C[j - 1] = sibling.C[j]

        child.n += 1
        sibling.n -= 1

    def mergeChild(self, i):
        child = self.C[i]
        sibling = self.C[i + 1]

        # Pull the key from the parent down into the child.
        child.keys[self.t - 1] = self.keys[i]

        # Copy keys from the sibling to the child.
        for j in range(sibling.n):
            child.keys[j + self.t] = sibling.keys[j]

        if not child.leaf:
            # Copy child pointers from the sibling to the child.
            for j in range(sibling.n + 1):
                child.C[j + self.t] = sibling.C[j]

        # Shift keys in the parent to fill the gap left by the key that was moved.
        for j in range(i + 1, self.n):
            self.keys[j - 1] = self.keys[j]

        # Shift child pointers in the parent to fill the gap.
        for j in range(i + 2, self.n + 1):
            self.C[j - 1] = self.C[j]

        child.n += sibling.n + 1
        self.n -= 1

class BTree:
    def __init__(self,t):
        self.root = None
        self.t = t
    
    def traverse(self,user):
        if self.root != None:
            self.root.traverse(0,user)
            
    def traverseStats(self):
        if self.root != None:
            self.root.traverseStats(0)
            
    def traverseSave(self,toSave):
        if self.root != None:
            self.root.traverseSave(0,toSave)
    
    def search(self,k):
        return None if self.root == None else self.root.search(k)
    
    def insert(self, data):
        if self.root == None: 
            self.root = BTreeNode(self.t,True) 
            self.root.keys[0] = data 
            self.root.n = 1 
        else:
            if self.root.n == 2 * self.t - 1: 
                s = BTreeNode(self.t,False) 
                s.C[0] = self.root 
                s.splitChild(0, self.root) 
                i = 0 
                if s.keys[0].name < data.name: 
                    i+=1 
                s.C[i].insertNonFull(data) 
                self.root = s 
            else: 
                self.root.insertNonFull(data) 
                
    def delete(self, key):
        if self.root is not None:
            self.root.delete(key)
            if self.root.n == 0:
                if self.root.leaf:
                    self.root = None
                else:
                    self.root = self.root.C[0]
   
   
def readData(fileSys, users,file_path = 'p10files.csv'):
    data_list = []
    with open(file_path, 'r') as file:
        for line in file:
            parts = line.strip().split(',')
            if len(parts) == 3:
                name, content, access_level = parts
                access_level = int(access_level)
                data = Data(name, content, access_level,users[random.randrange(0, len(users))].username)
                data_list.append(data)
    
    for data in data_list:
        #print(data)
        fileSys.insert(data)   
 
def readUsers(filename = 'p10users.csv'):
    users = []

    with open(filename, 'r') as file:
        for line in file:
            parts = line.strip().split(',')
            if len(parts) == 3:
                username, password, access_level = parts
                access_level = int(access_level)
                user = User(username, password, access_level)
                users.append(user)

    return users


def createFile(fileSys,user):
    fn = input("Type the file name and its extension (Ex: vscode.csv): ")
    if '.' not in fn:
        print("Invalid file name (No extension)")
        return
    print("Type the content of the file: ")
    print()
    
    content = input()
    
    print("Type the access level ")
    print()
    print("[7] - Only you and the admins can see this file")
    print("[6] - Any user can see this file but no one can open it")
    print("[5] - Any user can open this file")
    print("[4] - Any user can append in this file")
    print("[3] - Any user can overwrite this file")
    print("[2] - Any change the permissions of this file")
    print("[1] - Any user can delete this file")
    
    a = int(input())
    
    if(a < 1 or a > 7):
        print("That is not a valid permission, setting to default (5)")
        a = 5
        return
    
    toInsert = Data(fn,content,a,user.username)
    fileSys.insert(toInsert)

def searchFile(fileSys,user):
    toSearch = ""
    print()
    print("Type the file name (extension included)")
    print("Example: myFile.txt")
    toSearch = input()
    
    data = fileSys.search(toSearch)
    
    if data is None:
        print("The file doesn't exist")
        return None
    elif data.owner == user.username:
        print("File found!")
        print(data)
        print()
        print("Do you wanna open/delete this file?")
        print("[1] - Open")
        print("[2] - Delete")
        print("[3] - Return to menu")
        op = int(input())
        if op == 1:
            data.open(user)
        elif op == 2:
            if user.username == data.owner:
                print("Are you sure to delete this file?")
                print("[1] - YES // [2] - NO")
                op = int(input())
                if op == 1:
                    fileSys.delete(data.name)
                else: 
                    return
            elif data.access_level > user.access_level:
                print("You don't have permission to delete this file")
            else:
                print("Are you sure to delete this file?")
                print("[1] - YES // [2] - NO")
                op = int(input())
                if op == 1:
                    fileSys.delete(data.name)
                else: 
                    return
        else:
            return
    elif data.owner != user.username:
        if data.access_level > user.access_level:
            print("You don't have permission to access this file")
        else:
            print("File found!")
            print(data)
            print()
            print("Do you wanna open/delete this file?")
            print("[1] - Open")
            print("[2] - Delete")
            print("[3] - Return to menu")
            op = int(input())
            if op == 1:
                data.open(user)
            elif op == 2:
                if user.username == data.owner:
                    print("Are you sure to delete this file?")
                    print("[1] - YES // [2] - NO")
                    op = int(input())
                    if op == 1:
                        fileSys.delete(data.name)
                    else: 
                        return
                elif data.access_level > user.access_level:
                    print("You don't have permission to delete this file")
                else:
                    print("Are you sure to delete this file?")
                    print("[1] - YES // [2] - NO")
                    op = int(input())
                    if op == 1:
                        fileSys.delete(data.name)
                    else: 
                        return
            else:
                return

def menu(fileSys,user):
    global y
    global ax
    option = 0
    print("------- MENU --------")
    print()
    print("Choose an option by typing its number... ")
    print()
    print("1.- Change account/Logout")
    print("2.- Search file")
    print("3.- New file")
    print("4.- Show files")
    print("5.- Show statistics")
    print("6.- Exit")
    
    option = int(input())
    
    if option == 1:
        return 1
    elif option == 2:
        searchFile(fileSys,user)
        return 0
    elif option == 3:
        createFile(fileSys,user)
        return 0
    elif option == 4:
        fileSys.traverse(user)
        return 0
    elif option == 5:
        fileSys.traverseStats()
        plt.pie(y, labels = ax, shadow=True)
        plt.legend(title = "Access levels")
        plt.show() 
        return 0
    elif option == 6:
        return 2
    else:
        print("That is not a valid option")
        return 0
        
def login(fileSys,users,c,w):
    print("Login to NoHaxJustFilez system")
    print()
    flag = True
    logged = False
    session = None
    while flag:
        logU = input('Username: ')
        for i in range (len(users)):
            if users[i].username == logU:
                logP = input('Password: ')
                if users[i].password == logP:
                    print("Logged successfully")
                    c += 1
                    flag = False
                    logged = True
                    menuFlag = 0
                    while menuFlag == 0:
                        menuFlag = menu(fileSys,users[i])
                else:
                    print("Invalid password")
                    w += 1
                    
        if flag == True and logged == False and (logU not in users):
            print("Invalid username")
            w += 1
            print("Try again? (1: Yes, 2: No)")
            ch = int(input())
            if ch == 1:
                flag = True
            else:
                flag = False
        elif logged == True and menuFlag == 1:
            flag = True
            
    return c,w

def saveDataToFile(data_list, file_path='p10files.csv'):
    with open(file_path, 'w', newline='') as file:
        writer = csv.writer(file)
        for data in data_list:
            writer.writerow([data.name, data.content, data.access_level])


def main():
    users = readUsers()
    fileSys = BTree(10)
    readData(fileSys,users)
    c = 0
    w = 0
    c,w = login(fileSys,users,c,w)
    toSave = []
    
    # Execution time summary
    cat = ["Correct Login", "Wrong Login"]
    val = [c,w]
    
    
    fig,axe = plt.subplots()
    wi = 0.5
    x = range(len(cat))
    
    bar = axe.bar(x,val,width = wi)
    
    axe.set_xticks([i for i in x])
    axe.set_xticklabels(cat)
    
    axe.set_xlabel("Type of login")
    axe.set_ylabel("Amount of logs")
    axe.set_title("Login summary")
    
    plt.show()
    fileSys.traverseSave(toSave)
    saveDataToFile(toSave)
    
main()   