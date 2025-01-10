import streamlit as st
import datetime
import socket
import json
import time

def send_request(data):
    """Send request to C backend server"""
    HOST = 'localhost'
    PORT = 8080
    
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.settimeout(5)  # Set timeout to 5 seconds
            try:
                s.connect((HOST, PORT))
            except ConnectionRefusedError:
                st.error("Cannot connect to the server. Please make sure the server is running.")
                return {"error": "connection_failed"}
            except socket.timeout:
                st.error("Connection timeout. Please check if the server is running.")
                return {"error": "connection_timeout"}
            
            s.sendall(json.dumps(data).encode())
            try:
                response = s.recv(2048).decode()
                return json.loads(response)
            except json.JSONDecodeError:
                st.error("Invalid response from server")
                return {"error": "invalid_response"}
    except Exception as e:
        st.error(f"An error occurred: {str(e)}")
        return {"error": "unknown_error"}

def check_server():
    """Check if the server is running"""
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.settimeout(2)
            result = s.connect_ex(('localhost', 8080))
            return result == 0
    except:
        return False

def login_page():
    st.title("Bank Account System - Login")
    
    if not check_server():
        st.error("Server is not running! Please start the server first.")
        st.info("To start the server:")
        st.code("1. Open Command Prompt\n2. Navigate to server directory\n3. Run: bank_server.exe")
        return
    
    username = st.text_input("Username")
    password = st.text_input("Password", type="password")
    
    col1, col2 = st.columns(2)
    
    with col1:
        if st.button("Login"):
            if not username or not password:
                st.warning("Please enter both username and password")
                return
                
            response = send_request({
                "action": "login",
                "username": username,
                "password": password
            })
            
            if "error" in response:
                return
            
            if response.get("success"):
                st.session_state.logged_in = True
                st.session_state.username = username
                st.rerun()
            else:
                st.error("Invalid username or password")
    
    with col2:
        if st.button("Create Account"):
            st.session_state.page = "create_account"
            st.rerun()

def create_account_page():
    st.title("Create Bank Account")
    
    if not check_server():
        st.error("Server is not running! Please start the server first.")
        st.info("To start the server:")
        st.code("1. Open Command Prompt\n2. Navigate to server directory\n3. Run: bank_server.exe")
        return
    
    with st.form("create_account_form"):
        col1, col2 = st.columns(2)
        
        with col1:
            fname = st.text_input("First Name")
            lname = st.text_input("Last Name")
            fathname = st.text_input("Father's Name")
            mothname = st.text_input("Mother's Name")
            
        with col2:
            address = st.text_input("Address")
            account_type = st.selectbox("Account Type", ["savings", "current"])
            dob = st.date_input("Date of Birth", value=None, min_value=datetime.date(1900, 1, 1))
            phone = st.text_input("Phone Number")
        
        adhar = st.text_input("Adhar Number")
        username = st.text_input("Username")
        password = st.text_input("Password", type="password")
        
        submit_button = st.form_submit_button("Create Account")  # Ensure this is inside the form
        
        if submit_button:
            if not all([fname, lname, fathname, mothname, address, phone, adhar, username, password]):
                st.error("Please fill all fields")
                return
                
            response = send_request({
                "action": "create_account",
                "user_data": {
                    "fname": fname,
                    "lname": lname,
                    "fathname": fathname,
                    "mothname": mothname,
                    "address": address,
                    "typeaccount": account_type,
                    "date": dob.day,
                    "month": dob.month,
                    "year": dob.year,
                    "pnumber": phone,
                    "adharnum": adhar,
                    "username": username,
                    "password": password
                }
            })
            
            if "error" in response:
                return
                
            if response.get("success"):
                st.success("Account created successfully!")
                time.sleep(2)
                st.session_state.page = "login"
                st.rerun()
            else:
                st.error("Error creating account")

def dashboard_page():
    if not check_server():
        st.error("Lost connection to server! Please restart the server.")
        st.session_state.logged_in = False
        st.rerun()
        return
        
    st.title(f"Welcome, {st.session_state.username}")
    
    tab1, tab2 = st.tabs(["Account Info", "Transactions"])
    
    with tab1:
        response = send_request({
            "action": "get_user_details",
            "username": st.session_state.username
        })
        
        if "error" not in response:
            user_data = response.get("user_data")
            if user_data:
                col1, col2 = st.columns(2)
                
                with col1:
                    st.write(f"**Name:** {user_data['fname']} {user_data['lname']}")
                    st.write(f"**Father's Name:** {user_data['fathname']}")
                    st.write(f"**Mother's Name:** {user_data['mothname']}")
                    st.write(f"**Address:** {user_data['address']}")
                    
                with col2:
                    st.write(f"**Account Type:** {user_data['typeaccount']}")
                    st.write(f"**Phone:** {user_data['pnumber']}")
                    st.write(f"**Adhar:** {user_data['adharnum']}")
                    st.write(f"**DOB:** {user_data['date']}-{user_data['month']}-{user_data['year']}")
    
    with tab2:
        col1, col2 = st.columns(2)
        
        with col1:
            st.subheader("Check Balance")
            if st.button("Refresh Balance"):
                response = send_request({
                    "action": "check_balance",
                    "username": st.session_state.username
                })
                
                if "error" not in response:
                    st.write(f"**Total Balance:** ₹{response['total_balance']}")
                    
                    st.write("**Recent Transactions:**")
                    for tx in response.get("transactions", []):
                        st.write(f"From: {tx['from']}, Amount: ₹{tx['amount']}")
        
        with col2:
            st.subheader("Transfer Money")
            with st.form("transfer_form"):
                to_user = st.text_input("To Username")
                amount = st.number_input("Amount", min_value=1)
                
                if st.form_submit_button("Transfer"):
                    if not to_user:
                        st.error("Please enter recipient username")
                        return
                        
                    response = send_request({
                        "action": "transfer",
                        "from_user": st.session_state.username,
                        "to_user": to_user,
                        "amount": amount
                    })
                    
                    if "error" not in response:
                        if response.get("success"):
                            st.success("Transfer successful!")
                        else:
                            st.error("Transfer failed")

def main():
    st.set_page_config(
        page_title="Bank Management System",
        page_icon="🏦",
        initial_sidebar_state="expanded"
    )
    
    if "page" not in st.session_state:
        st.session_state.page = "login"
    
    if "logged_in" not in st.session_state:
        st.session_state.logged_in = False
    
    if st.session_state.logged_in:
        dashboard_page()
        if st.sidebar.button("Logout"):
            st.session_state.logged_in = False
            st.session_state.username = None
            st.session_state.page = "login"
            st.rerun()
    else:
        if st.session_state.page == "login":
            login_page()
        elif st.session_state.page == "create_account":
            create_account_page()

if __name__ == "__main__":
    main()
