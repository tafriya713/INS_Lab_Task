import os
import time
import hashlib
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
from Crypto.Cipher import AES, PKCS1_OAEP
from Crypto.PublicKey import RSA
from Crypto.Random import get_random_bytes
from Crypto.Util.Padding import pad, unpad
from Crypto.Signature import pkcs1_15
from Crypto.Hash import SHA256


def generate_aes_key(bits):
    key = get_random_bytes(bits // 8)
    with open(f"aes_key_{bits}.bin", "wb") as f:
        f.write(key)
    print(f"AES-{bits} key generated and saved to aes_key_{bits}.bin")
    return key


def aes_encrypt():
    bits = int(input("Enter AES key size (128 or 256): "))
    mode = input("Enter mode (ECB or CFB): ").upper()
    input_file = input("Enter the file name to encrypt (e.g., test.txt): ")

    try:
        with open(input_file, "r") as f:
            plaintext = f.read()
    except FileNotFoundError:
        print(" File not found. Make sure it’s in the same folder as this program.")
        return

    key = generate_aes_key(bits)

    if mode == "ECB":
        cipher = AES.new(key, AES.MODE_ECB)
    elif mode == "CFB":
        iv = get_random_bytes(16)
        cipher = AES.new(key, AES.MODE_CFB, iv)
    else:
        print(" Invalid mode. Choose ECB or CFB.")
        return

    start_time = time.time()

    ciphertext = cipher.encrypt(pad(plaintext.encode(), AES.block_size))
    with open("aes_encrypted.bin", "wb") as f:
        if mode == "CFB":
            f.write(iv)
        f.write(ciphertext)

    end_time = time.time()

    print(f" AES-{bits} {mode} encryption done.")
    print(f" Execution time: {end_time - start_time:.6f} seconds\n")


def aes_decrypt():
    bits = int(input("Enter AES key size (128 or 256): "))
    mode = input("Enter mode (ECB or CFB): ").upper()
    key_file = f"aes_key_{bits}.bin"

    try:
        with open(key_file, "rb") as f:
            key = f.read()
        with open("aes_encrypted.bin", "rb") as f:
            data = f.read()
    except FileNotFoundError:
        print(" Required file(s) not found.")
        return

    start_time = time.time()

    if mode == "CFB":
        iv = data[:16]
        ciphertext = data[16:]
        cipher = AES.new(key, AES.MODE_CFB, iv)
        plaintext = cipher.decrypt(ciphertext).decode()
    else:
        cipher = AES.new(key, AES.MODE_ECB)
        plaintext = unpad(cipher.decrypt(data), AES.block_size).decode()

    end_time = time.time()

    print("\n Decrypted text:")
    print(plaintext)
    print(f"\n Execution time: {end_time - start_time:.6f} seconds\n")


def generate_rsa_keys():
    key = RSA.generate(2048)
    private_key = key.export_key()
    public_key = key.publickey().export_key()

    with open("private.pem", "wb") as f:
        f.write(private_key)
    with open("public.pem", "wb") as f:
        f.write(public_key)

    print(" RSA keys generated: private.pem & public.pem\n")


def rsa_encrypt():
    input_file = input("Enter file name to encrypt (e.g., test.txt): ")

    try:
        with open(input_file, "r") as f:
            plaintext = f.read().encode()
        with open("public.pem", "rb") as f:
            public_key = RSA.import_key(f.read())
    except FileNotFoundError:
        print(" File not found.")
        return

    cipher = PKCS1_OAEP.new(public_key)

    if len(plaintext) > 190:  # RSA can’t handle large data directly
        print(" File too large for direct RSA encryption. Use smaller text file.")
        return

    start_time = time.time()
    ciphertext = cipher.encrypt(plaintext)
    end_time = time.time()

    with open("rsa_encrypted.bin", "wb") as f:
        f.write(ciphertext)

    print(f" RSA encryption done. ⏱ Time: {end_time - start_time:.6f} seconds\n")


def rsa_decrypt():
    try:
        with open("private.pem", "rb") as f:
            private_key = RSA.import_key(f.read())
        with open("rsa_encrypted.bin", "rb") as f:
            ciphertext = f.read()
    except FileNotFoundError:
        print(" Missing RSA key or encrypted file.")
        return

    cipher = PKCS1_OAEP.new(private_key)
    start_time = time.time()
    plaintext = cipher.decrypt(ciphertext)
    end_time = time.time()

    print(f"\n Decrypted text:\n{plaintext.decode()}")
    print(f"Execution time: {end_time - start_time:.6f} seconds\n")


def rsa_signature():
    input_file = input("Enter file to sign (e.g., test.txt): ")
    try:
        with open(input_file, "rb") as f:
            message = f.read()
        with open("private.pem", "rb") as f:
            private_key = RSA.import_key(f.read())
    except FileNotFoundError:
        print(" File not found.")
        return

    h = SHA256.new(message)
    signature = pkcs1_15.new(private_key).sign(h)

    with open("signature.bin", "wb") as f:
        f.write(signature)

    print(" RSA signature created and saved to signature.bin\n")


def verify_signature():
    input_file = input("Enter file to verify (e.g., test.txt): ")
    try:
        with open(input_file, "rb") as f:
            message = f.read()
        with open("signature.bin", "rb") as f:
            signature = f.read()
        with open("public.pem", "rb") as f:
            public_key = RSA.import_key(f.read())
    except FileNotFoundError:
        print(" File(s) missing for verification.")
        return

    h = SHA256.new(message)
    try:
        pkcs1_15.new(public_key).verify(h, signature)
        print(" Signature is valid.\n")
    except (ValueError, TypeError):
        print(" Signature is invalid.\n")


def sha256_hash():
    input_file = input("Enter file to hash (e.g., test.txt): ")
    try:
        with open(input_file, "rb") as f:
            data = f.read()
    except FileNotFoundError:
        print(" File not found.")
        return

    hash_value = hashlib.sha256(data).hexdigest()
    print(f" SHA-256 hash:\n{hash_value}\n")

def measure_time():
    import matplotlib.pyplot as plt
    from Crypto.Cipher import AES
    from Crypto.PublicKey import RSA
    from Crypto.Random import get_random_bytes
    from Crypto.Util.Padding import pad
    import time

    # Only valid sizes for AES and RSA
    aes_key_sizes = [128, 256]
    rsa_key_sizes = [1024, 2048, 3072, 4096]

    aes_times = []
    rsa_times = []

    print(" Measuring AES and RSA performance...")

    # --- AES timing ---
    for bits in aes_key_sizes:
        key = get_random_bytes(bits // 8)
        cipher = AES.new(key, AES.MODE_ECB)
        data = b"A" * 64
        start = time.time()
        cipher.encrypt(pad(data, AES.block_size))
        end = time.time()
        aes_times.append(end - start)

    # --- RSA timing ---
    for bits in rsa_key_sizes:
        start = time.time()
        key = RSA.generate(bits)
        end = time.time()
        rsa_times.append(end - start)

    # --- Plotting ---
    plt.figure()
    plt.plot(aes_key_sizes, aes_times, marker="o", label="AES (128/256)")
    plt.plot(rsa_key_sizes, rsa_times, marker="s", label="RSA (1024–4096)")
    plt.xlabel("Key size (bits)")
    plt.ylabel("Execution time (seconds)")
    plt.title("Execution Time Comparison: AES vs RSA")
    plt.legend()
    plt.grid(True)
    plt.show()

while True:
    print("\n=== CRYPTO LAB 4 MENU ===")
    print("1. AES Encryption")
    print("2. AES Decryption")
    print("3. RSA Key Generation")
    print("4. RSA Encryption")
    print("5. RSA Decryption")
    print("6. RSA Signature")
    print("7. Verify RSA Signature")
    print("8. SHA-256 Hash")
    print("9. Measure Execution Time")
    print("10. Exit")

    choice = input("Enter choice: ")

    if choice == '1':
        aes_encrypt()
    elif choice == '2':
        aes_decrypt()
    elif choice == '3':
        generate_rsa_keys()
    elif choice == '4':
        rsa_encrypt()
    elif choice == '5':
        rsa_decrypt()
    elif choice == '6':
        rsa_signature()
    elif choice == '7':
        verify_signature()
    elif choice == '8':
        sha256_hash()
    elif choice == '9':
        measure_time()
    elif choice == '10':
        print("Exiting...")
        break
    else:
        print(" Invalid choice.")
