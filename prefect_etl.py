from prefect import task, Flow
import prefect.executors
import pandas as pd

@task
def extract_data():
    return pd.read_csv('sales_data.csv')

@task
def transform_chunk(chunk):
    return chunk.groupby('Product')['Sales'].sum().reset_index()
  
@task
def aggregate_data(transformed_chunks):
    return pd.concat(transformed_chunks).groupby('Product')['Sales'].sum().result()

@task
def load_data(aggregated_data):
    # Dummy function to represent data loading
    conn  = sqlite3.connect('sales.db')
    data.to_sql('sales_dummy',conn,if_exists='replace',index=False)
    conn.close()

if __name__ == "__main__":
    with Flow("SalesETL") as flow:
        data = extract_data()
        chunk_size = 1000

        data_chunks = [data[i:i + chunk_size] for i in range(0, len(data), chunk_size)]
        transformed_chunks = []

        for chunk in data_chunks:
            transformed_chunk = transform_chunk(chunk)
            transformed_chunks.append(transformed_chunk)

        aggregated_data = aggregate_data(transformed_chunks)
        load_data(aggregated_data)

    executor = prefect.executors.LocalDaskExecutor()
    flow_state = flow.run(executor=executor)

    if flow_state.is_successful():
        print("Sales ETL pipeline executed successfully.")
