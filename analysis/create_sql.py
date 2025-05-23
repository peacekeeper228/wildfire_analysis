sql = []
with open("data/fires_after.csv", 'r') as f:
    for i in f.readlines():
        sql.append(f"""INSERT INTO FIRE_After SELECT 100, ST_AsText(ST_Transform(ST_Translate(ST_Transform(ST_SetSRID(ST_MakePoint('-124','54'), 4326), 32654), -(630+{i.split(',')[1]})*30, -(530+{i.split(',')[0]})*40), 4326));""")

with open("data.txt", "w+") as f:
    f.writelines(sql)