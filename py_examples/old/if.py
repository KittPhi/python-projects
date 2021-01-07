def func(country, total):
    if country == "US":
        if total <= 50:
            print("Shipping Cost is  $50")
        elif total <= 100:
                print("Shipping Cost is $25")
        elif total <= 150:
                print("Shipping Costs $5")
        else:
                print("FREE")
    if country == "AU": 
        if total <= 50:
            print("Shipping Cost is  $100")
    else:
            print("FREE")

if __name__ == "__main__":
    func("US", 100)